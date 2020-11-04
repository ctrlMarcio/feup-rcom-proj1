/**
 * @file    error.h
 * @brief   Defines the error flags.
 * @date 2020-10-11
 */

#pragma once

/**
 * @brief Defines the error code for errors related to the arguments of the program.
 */
#define SENDER_ARGS_ERROR -1

#define RECEIVER_ARGS_ERROR -2

/**
 * @brief Defines the error code for errors related to opening and config ports
 */
#define CONFIG_PORT_ERROR -3

#define ESTABLISH_CONNECTION_ERROR -4

#define CLOSE_CONNECTION_ERROR -5

#define LOST_FRAME_ERROR -6

#define LOST_START_PACKET_ERROR -7

#define LOST_END_PACKET_ERROR -8

#define LOST_DATA_PACKET_ERROR -9

#define INVALID_CTRL_PACKET_TYPE -10

#define FILE_ERROR -11

#define EXISTING_FILE_ERROR -12

#define REPEATED_FRAME_ERROR -13

void print_error(int error_code);

void print_error_message(int error_code, char *arg);