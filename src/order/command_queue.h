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

#ifndef COMMAND_QUEUE_H_
#define COMMAND_QUEUE_H_

#include <inttypes.h>
#include <stdbool.h>

void cmd_queue_init(void);
void cmd_queue_add_blocks(uint_fast8_t parameter_length);
void cmd_queue_clear(void);
void cmd_queue_tick(void);
void cmd_queue_reset_executed_commands(void);
bool cmd_queue_chnage_setting(uint8_t* setting);
#ifdef DEBUG_ACTIVE
bool cmd_queue_show_status(uint8_t* setting);
#endif

#endif /* COMMAND_QUEUE_H_ */
