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

#define RECEIVE_BUFFER_SIZE_FRAMES  2
#define RECEIVE_BUFFER_SIZE_BYTES (MAX_RECEIVE_FRAME_SIZE * RECEIVE_BUFFER_SIZE_FRAMES)
#define endPos (RECEIVE_BUFFER_SIZE_BYTES -1)
#define PORT 54321

static void* uart_task(void * arg);

static uint_fast8_t receive_buffer[RECEIVE_BUFFER_SIZE_BYTES];
static uint_fast16_t read_pos;
static uint_fast16_t write_pos;
static bool i_want_to_send;
static uint_fast8_t * send_frame;
static uint_fast16_t send_length;
pthread_t uart_thread;
pthread_mutex_t receive_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sending_done   = PTHREAD_COND_INITIALIZER;

bool hal_uart_init(void)
{
    read_pos = 0;
    write_pos = 0;
    i_want_to_send = false;
    int ret = pthread_create( &uart_thread, NULL, uart_task, NULL);
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        return false;
    }
    return true;
}

static void* uart_task(void * arg)
{
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
    my_addr.sin_port = htons(PORT);
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
            pthread_mutex_lock(&receive_mutex);
            length = recv(client_fd, &receive_buffer[write_pos], RECEIVE_BUFFER_SIZE_BYTES - write_pos, 0);
            if(-1 == length)
            {
                if((EAGAIN == errno) || (EWOULDBLOCK == errno))
                {
                    // ok
                }
                else
                {
                    fprintf(stderr, "Connection closed by remote host.\n");
                    pthread_mutex_unlock(&receive_mutex);
                    break;
                }
            }
            else if(0 < length)
            {
                // we received something
                write_pos += length;
                if(write_pos > endPos)
                {
                    write_pos = write_pos - RECEIVE_BUFFER_SIZE_BYTES;
                }
            }
            pthread_mutex_unlock(&receive_mutex);

            fprintf(stderr, "send\n");
            // send
            pthread_mutex_lock(&send_mutex);
            if(true == i_want_to_send)
            {
                send(client_fd, send_frame, send_length, 0);
                i_want_to_send = false;
                pthread_cond_signal( &sending_done );
            }
            pthread_mutex_unlock(&send_mutex);

            fprintf(stderr, "wait\n");
            // do not use the whole CPU
            usleep(1000);
        }

        close(client_fd);
    }
    close(sock_fd);
    return NULL;
}


uint_fast8_t hal_uart_get_byte_at_offset(uint_fast16_t offset)
{
    uint_fast8_t res;
    pthread_mutex_lock(&receive_mutex);
    uint_fast16_t target_pos = read_pos + offset;
    if(endPos < target_pos)
    {
        target_pos = target_pos - endPos;
    }
    res = receive_buffer[target_pos];
    pthread_mutex_unlock(&receive_mutex);
    return res;
}

uint_fast16_t hal_uart_get_available_bytes(void)
{
    uint_fast16_t res = 0;
    pthread_mutex_lock(&receive_mutex);
    if(read_pos != write_pos)
    {
        if(write_pos > read_pos)
        {
            res = write_pos - read_pos;
        }
        else
        {
            res = (endPos + 1) - read_pos + (0 - write_pos);
        }
    }
    // else res = 0;
    pthread_mutex_unlock(&receive_mutex);
    return res;
}

void hal_uart_forget_bytes(uint_fast16_t how_many)
{
    pthread_mutex_lock(&receive_mutex);
    uint_fast16_t target_pos = read_pos + how_many;
    if(endPos < target_pos)
    {
        target_pos = target_pos - endPos;
    }
    read_pos = target_pos;
    pthread_mutex_unlock(&receive_mutex);
}

void hal_uart_send_frame(uint_fast8_t * frame, uint_fast16_t length)
{
    // prepare to send
    send_frame = frame;
    send_length = length;
    pthread_mutex_lock(&send_mutex);
    i_want_to_send = true;
    pthread_mutex_unlock(&send_mutex);
    // wait until data has been send
    pthread_mutex_lock(&send_mutex);
    pthread_cond_wait(&sending_done, &send_mutex);
    pthread_mutex_unlock(&send_mutex);
}
