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

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// The values defined here are defined in the Pacemaker protocol.
// The Pacemaker protocol definition is available here:
// https://github.com/JustAnother1/Pacemaker/blob/master/doc/Pacemaker_Protocol.asciidoc

#define HOST_SYNC_REQUEST                                      0x23
#define CLIENT_FRAME_SYNC                                      0x42

#define REQUEST_NUMBER_OF_NON_PARAMETER_BYTES_IN_LENGTH        2


#define REPLY_FRAME_POS_OF_LENGTH                              1
#define REPLY_FRAME_POS_OF_CONTROL                             2
#define REPLY_FRAME_POS_OF_REPLY_CODE                          3
#define REPLY_FRAME_START_OF_PARAMETER                         4

#define REQUEST_FRAME_POS_OF_CONTROL                           2
#define REQUEST_FRAME_POS_OF_ORDER_CODE                        3
#define REQUEST_FRAME_START_OF_PARAMETER                       4

#define REQUEST_CONTROL_SEQUNECE_NUMBER_FLAG                   0x0f
#define REQUEST_CONTROL_RESYNC_FLAG_FLAG                       0x10
#define REPLY_CONTROL_FLAG_DEBUG                               0x80

#define MIN_BYTES_HOST_FRAME                                   5
#define MIN_BYTES_CLIENT_FRAME                                 5
#define MAX_BYTES_CLIENT_PARAMETER                             50  // TODO
#define MAX_RECEIVE_FRAME_SIZE                                 256
#define MAX_SEND_FRAME_SIZE                                    MIN_BYTES_CLIENT_FRAME + MAX_BYTES_CLIENT_PARAMETER

#define REPLY_CODE_FRAME_RECEIPT_ERROR                         0x00
#define FRAME_ERROR_BAD_FRAME                                  0
#define FRAME_ERROR_BAD_CRC                                    1
#define FRAME_ERROR_UNABLE_TO_ACCEPT                           2

#define REPLY_CODE_OK                                          0x10

#define REPLY_CODE_GENERIC_APPLICATION_ERROR                   0x11
#define GENERIC_ERROR_UNKNOWN_ORDER                            1
#define GENERIC_ERROR_BAD_PARAMETER_FORMAT                     2
#define GENERIC_ERROR_BAD_PARAMETER_VALUE                      3
#define GENERIC_ERROR_INVALID_DEVICE_TYPE                      4
#define GENERIC_ERROR_INVALID_DEVICE_NUMBER                    5
#define GENERIC_ERROR_INCORRECT_MODE                           6
#define GENERIC_ERROR_BUSY                                     7
#define GENERIC_ERROR_FAILED                                   8
#define GENERIC_ERROR_FIRMWARE_ERROR                           9

#define REPLY_CODE_STOPPED                                     0x12
#define RECOVERY_CONDITION_CLEARED                             1
#define RECOVERY_CONDITION_PERSIST                             2
#define RECOVERY_UNRECOVERABLE                                 3
#define STOPPED_CAUSE_RESET                                    0
#define STOPPED_CAUSE_END_STOP_HIT                             1
#define STOPPED_CAUSE_MOVEMENT_ERROR                           2
#define STOPPED_CAUSE_TEMPERATURE_ERROR                        3
#define STOPPED_CAUSE_DEVICE_FAULT                             4
#define STOPPED_CAUSE_ELECTRICAL_FAULT                         5
#define STOPPED_CAUSE_FIRMWARE_FAULT                           6
#define STOPPED_CAUSE_USER_REQUESTED                           7
#define STOPPED_CAUSE_HOST_TIMEOUT                             8
#define STOPPED_CAUSE_OTHER_FAULT                              9

#define REPLY_CODE_ORDER_SPECIFIC_ERROR                        0x13
#define QUEUE_CAUSE_QUEUE_FULL                                 1
#define QUEUE_CAUSE_UNKNOWN_COMMAND_BLOCK                      2
#define QUEUE_CAUSE_MALFORMED_BLOCK                            3
#define QUEUE_CAUSE_ERROR_IN_COMMAND_BLOCK                     4

#define DEFAULT_ERROR_REPLY_CODE                               0xff

#define REPLY_CODE_DEBUG                                       0x50

// orders
#define ORDER_RESUME                                           0x00
#define ORDER_REQUEST_INFORMATION                              0x01
#define ORDER_REQUEST_DEVICE_NAME                              0x02
#define ORDER_REQUEST_TEMPERATURE_READING                      0x03
#define ORDER_GET_HEATER_CONFIGURATION                         0x04
#define ORDER_CONFIGURE_HEATER                                 0x05
#define ORDER_SET_HEATER_TARGET_TEMPERATURE                    0x06
#define ORDER_REQUEST_STATE_OF_SWITCH                          0x07
#define ORDER_SWITCH_AN_OUTPUT                                 0x08
#define ORDER_SET_PWM_OUTPUT_VALUE                             0x09
#define ORDER_WRITE_FIRMWARE_CONFIGURATION_VALUE               0x0a
#define ORDER_READ_FIRMWARE_CONFIGURATION_VALUE                0x0b
#define ORDER_EMERGENCY_STOP                                   0x0c
#define ORDER_ACTIVATE_STEPPER_CONTROL                         0x0d
#define ORDER_ENABLE__DIABLE_STEPPER_MOTORS                    0x0e
#define ORDER_CONFIGURE_END_STOPS                              0x0f
#define ORDER_ENABLE__DISABLE_END_STOPS                        0x10
#define ORDER_REQUEST_DEVICE_COUNT                             0x11
#define ORDER_QUEUE_COMMAND_BLOCKS                             0x12
#define ORDER_CONFIGURE_AXIS_MOVEMENT_RATES                    0x13
#define ORDER_RETRIEVE_EVENTS                                  0x14
#define ORDER_GET_NUMBER_OF_EVENT_FORMAT_IDS                   0x15
#define ORDER_GET_EVENT_STRING_FORMAT_ID                       0x16
#define ORDER_CLEAR_COMMAND_BLOCK_QUEUE                        0x17
#define ORDER_REQUEST_DEVICE_STATUS                            0x18
#define ORDER_CONFIGURE_MOVEMENT_UNDERRUN_AVOIDANCE_PARAMETERS 0x19
#define ORDER_GET_FIRMWARE_CONFIGURATION_VALUE_PROPERTIES      0x1a
#define ORDER_TRAVERSE_FIMRWARE_CONFIGURATION_VALUES           0x1b
#define ORDER_SET_OUTPUT_TONE                                  0x1c
#define ORDER_RESET                                            0x7f

// device Types
#define DEVICE_TYPE_INVALID                                    0x00
#define DEVICE_TYPE_SWITCH_INPUT                               0x01
#define DEVICE_TYPE_SWITCH_OUTPUT                              0x02
#define DEVICE_TYPE_PWM_OUTPUT                                 0x03
#define DEVICE_TYPE_STEPPER                                    0x04
#define DEVICE_TYPE_HEATER                                     0x05
#define DEVICE_TYPE_TEMPERATURE_SENSOR                         0x06
#define DEVICE_TYPE_BUZZER                                     0x07

// device Status:
#define DEVICE_STATUS_ACTIVE                                   0x00
#define DEVICE_STATUS_INACTIVE                                 0x01
#define DEVICE_STATUS_DISABLED                                 0x02
#define DEVICE_STATUS_CONFIGURATION_ERROR                      0x03
#define DEVICE_STATUS_FAULT                                    0x04
#define DEVICE_STATUS_CLIENT_STOPPED                           0x05

#define SWITCH_STATE_NOT_TRIGGERED                             0
#define SWITCH_STATE_TRIGGERED                                 1
#define OUTPUT_STATE_LOW                                       0
#define OUTPUT_STATE_HIGH                                      1
#define OUTPUT_STATE_HIGH_Z                                    2

#define MOVEMENT_BLOCK_TYPE_COMMAND_WRAPPER                    1
#define MOVEMENT_BLOCK_TYPE_DELAY                              2
#define MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE                  3
#define MOVEMENT_BLOCK_TYPE_SET_ACTIVE_TOOLHEAD                4
#define MOVEMENT_BLOCK_TYPE_MOVEMENT_CHECKPOINT                5

#endif /* PROTOCOL_H_ */
