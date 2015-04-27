/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include "stddef.h"
#include "pthread.h"
#include "hal_uart.h"
#include "protocol.h"
#include "hal_cfg.h"
#include "board_cfg.h"
#include "hal_debug.h"

#define RECEIVE_BUFFER_SIZE_FRAMES  2
#define RECEIVE_BUFFER_SIZE_BYTES (MAX_RECEIVE_FRAME_SIZE * RECEIVE_BUFFER_SIZE_FRAMES)
#define endPos (RECEIVE_BUFFER_SIZE_BYTES -1)
#define PORT 54321

typedef struct {
    bool is_std_io;
    uint_fast16_t port;
    volatile uint_fast16_t read_pos;
    volatile uint_fast16_t write_pos;
    bool i_want_to_send;
    pthread_t uart_thread;
    uint_fast8_t receive_buffer[RECEIVE_BUFFER_SIZE_BYTES];
    uint_fast8_t * send_frame;
    uint_fast16_t send_length;
    pthread_mutex_t receive_mutex;
    pthread_mutex_t send_mutex;
    pthread_cond_t  sending_done;
}uart_device_typ;


static uart_device_typ devices[MAX_UART];

static void* uart_task(void * dev_ptr);
static void* uart_std_task(void * dev_ptr);

void hal_uart_print_configuration(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        debug_line("Configuration of UART_%d :", device);
        if(true == devices[device].is_std_io)
        {
            debug_line(" is a standard io interface");
        }
        else
        {
            debug_line(" is a telnet interface");
        }
    }
    // else invalid Interface Specified
}

bool hal_uart_init(uint_fast8_t device, uint_fast16_t rec_buf_size, uint_fast16_t send_buf_size)
{
    if(device < MAX_UART)
    {
        int ret = 0;
        if((0 > device) || (MAX_UART <= device))
        {
            fprintf(stderr,"Device number out of range! Device number: %d\n", device);
            return false;
        }
        fprintf(stdout,"Initializing the Device number: %d\n", device);
        switch(device)
        {
        case 0: devices[device].is_std_io = UART_0_IS_STD_IO; break;
        case 1: devices[device].is_std_io = UART_1_IS_STD_IO; break;
        case 2: devices[device].is_std_io = UART_2_IS_STD_IO; break;
        case 3: devices[device].is_std_io = UART_3_IS_STD_IO; break;
        case 4: devices[device].is_std_io = UART_4_IS_STD_IO; break;
        default: devices[device].is_std_io = false;
        }
        devices[device].port = PORT + device;
        devices[device].read_pos = 0;
        devices[device].write_pos = 0;
        devices[device].i_want_to_send = false;
        pthread_mutex_init(&devices[device].receive_mutex, NULL);
        pthread_mutex_init(&devices[device].send_mutex, NULL);
        pthread_cond_init(&devices[device].sending_done, NULL);
        if(false == devices[device].is_std_io)
        {
            // listen to TCP connection
            ret = pthread_create(&devices[device].uart_thread,
                                 NULL,
                                 uart_task,
                                 &devices[device]);
        }
        else
        {
            // use std streams
            ret = pthread_create(&devices[device].uart_thread,
                                 NULL,
                                 uart_std_task,
                                 &devices[device]);
        }
        if(ret)
        {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
            return false;
        }
        return true;
    }
    else
    {
        // invalid Interface Specified
        return false;
    }
}

static void* uart_std_task(void * dev_ptr)
{
    uart_device_typ * device = dev_ptr;
    int c;
    fprintf(stdout,"Standard I/O task started,...\n");
    while((c = fgetc(stdin)) != EOF)
    {
        pthread_mutex_lock(&device->receive_mutex);
        device->receive_buffer[device->write_pos] = c;
        device->write_pos ++;
        if(device->write_pos > endPos)
        {
            device->write_pos = 0;
        }
        pthread_mutex_unlock(&device->receive_mutex);
        // do not use the whole CPU
        usleep(1000);
    }
    return NULL;
}

static void* uart_task(void * dev_ptr)
{
    uart_device_typ * device = dev_ptr;
    int sock_fd;
    int client_fd;
    int err;
    int length;
    socklen_t addr_size;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;

    // socket
    sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == sock_fd)
    {
        perror("pmc:");
        fprintf(stderr, "Can't create new socket\n");
        exit(1);
        return NULL;
    }
    int flag = 1;
    int result = setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    if(0 > result)
    {
        perror("pmc:");
        fprintf(stderr, "Can't set nodelay\n");
        exit(1);
        return NULL;
    }

    // bind
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(device->port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    err = bind(sock_fd,
              (struct sockaddr *)&my_addr,
              sizeof(struct sockaddr_in)   );
    if(-1 == err)
    {
        perror("pmc:");
        fprintf(stderr, "bind() failed\n");
        exit(1);
        return NULL;
    }

    while(1)
    {
        int opts;
        // listen
        err = listen(sock_fd, 1);
        if(-1 == err)
        {
            perror("pmc:");
            fprintf(stderr, "listen() failed\n");
            exit(1);
            return NULL;
        }

        // accept
        addr_size = sizeof(struct sockaddr_in);
        client_fd = accept(sock_fd,
                           (struct sockaddr *)&client_addr,
                           &addr_size);
        if(-1 == client_fd)
        {
            perror("pmc:");
            fprintf(stderr, "accept() failed\n");
            exit(1);
            return NULL;
        }

        opts = fcntl(client_fd, F_GETFL);
        if(opts < 0)
        {
            perror("pmc:");
            fprintf(stderr, "non blocking failed\n");
            exit(1);
            return NULL;
        }
        opts = (opts | O_NONBLOCK);
        if(fcntl(client_fd, F_SETFL, opts) < 0)
        {
            perror("pmc:");
            fprintf(stderr, "non blocking failed\n");
            exit(1);
            return NULL;
        }

        printf("I'm connected from %s\n", inet_ntoa(client_addr.sin_addr));

        while (1)
        {
            fprintf(stderr, "receive\n");
            // receive
            pthread_mutex_lock(&device->receive_mutex);
            length = recv(client_fd,
                          &device->receive_buffer[device->write_pos],
                          RECEIVE_BUFFER_SIZE_BYTES - device->write_pos,
                          0);
            if(-1 == length)
            {
                if((EAGAIN == errno) || (EWOULDBLOCK == errno))
                {
                    // ok
                }
                else
                {
                    fprintf(stderr, "Connection closed by remote host.\n");
                    pthread_mutex_unlock(&device->receive_mutex);
                    break;
                }
            }
            else if(0 < length)
            {
                // we received something
                device->write_pos += length;
                if(device->write_pos > endPos)
                {
                    device->write_pos = device->write_pos - RECEIVE_BUFFER_SIZE_BYTES;
                }
            }
            pthread_mutex_unlock(&device->receive_mutex);

            fprintf(stderr, "send\n");
            // send
            pthread_mutex_lock(&device->send_mutex);
            if(true == device->i_want_to_send)
            {
                send(client_fd, device->send_frame, device->send_length, 0);
                device->i_want_to_send = false;
                pthread_cond_signal( &device->sending_done );
            }
            pthread_mutex_unlock(&device->send_mutex);

            fprintf(stderr, "wait\n");
            // do not use the whole CPU
            usleep(1000);
        }

        close(client_fd);
    }
    close(sock_fd);
    return NULL;
}


uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset)
{
    if(device < MAX_UART)
    {
        uint_fast8_t res;
        pthread_mutex_lock(&devices[device].receive_mutex);
        uint_fast16_t target_pos = devices[device].read_pos + offset;
        if(endPos < target_pos)
        {
            target_pos = target_pos - endPos;
        }
        res = devices[device].receive_buffer[target_pos];
        pthread_mutex_unlock(&devices[device].receive_mutex);
        return res;
    }
    else
    {
        // invalid Interface Specified
        return ' ';
    }
}

uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        uint_fast16_t res = 0;
        pthread_mutex_lock(&devices[device].receive_mutex);
        if(devices[device].read_pos != devices[device].write_pos)
        {
            if(devices[device].write_pos > devices[device].read_pos)
            {
                res = devices[device].write_pos - devices[device].read_pos;
            }
            else
            {
                res = (endPos + 1) - devices[device].read_pos + (0 - devices[device].write_pos);
            }
        }
        // else res = 0;
        pthread_mutex_unlock(&devices[device].receive_mutex);
        return res;
    }
    else
    {
        // invalid Interface Specified
        return 0;
    }
}

void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many)
{
    if(device < MAX_UART)
    {
        pthread_mutex_lock(&devices[device].receive_mutex);
        uint_fast16_t target_pos = devices[device].read_pos + how_many;
        if(endPos < target_pos)
        {
            target_pos = target_pos - endPos;
        }
        devices[device].read_pos = target_pos;
        pthread_mutex_unlock(&devices[device].receive_mutex);
    }
    // else invalid Interface Specified
}

bool hal_uart_send_frame_non_blocking(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    hal_uart_send_frame(device, frame, length);
    return true;
}

void hal_uart_send_frame(uint_fast8_t device, uint_fast8_t * frame, uint_fast16_t length)
{
    if(device < MAX_UART)
    {
        if(false == devices[device].is_std_io)
        {
            // prepare to send
            devices[device].send_frame = frame;
            devices[device].send_length = length;
            pthread_mutex_lock(&devices[device].send_mutex);
            devices[device].i_want_to_send = true;
            pthread_mutex_unlock(&devices[device].send_mutex);
            // wait until data has been send
            /* not working if no client connected
            pthread_mutex_lock(&devices[device].send_mutex);
            pthread_cond_wait(&devices[device].sending_done, &devices[device].send_mutex);
            pthread_mutex_unlock(&devices[device].send_mutex);
            */
        }
        else
        {
            // just write it to stdout
            uint_fast16_t i;
            for(i = 0; i < length; i++)
            {
                char c = frame[i];
                printf("%c",c);
            }
        }
    }
    // else invalid Interface Specified
}
