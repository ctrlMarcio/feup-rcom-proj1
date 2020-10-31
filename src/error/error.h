/**
 * @file    error.h
 * @brief   Defines the error flags.
 * @date 2020-10-11
 */

#pragma once

/**
 * @brief Defines the error code for errors related to the arguments of the program.
 */
#define ARGS_ERROR -1

/**
 * @brief Defines the error code for errors related to opening and config ports
 */
#define CONFIG_PORT_ERROR -2

#define ESTABLISH_CONNECTION_ERROR -3

#define CLOSE_CONNECTION_ERROR -4

#define LOST_FRAME_ERROR -5

#define LOST_START_PACKET_ERROR -6

#define LOST_END_PACKET_ERROR -7

#define LOST_DATA_PACKET_ERROR -8

#define INVALID_CTRL_PACKET_TYPE -9

#define FILE_ERROR -10

#define EXISTING_FILE_ERROR -11

void print_error(int error_code);

void print_error_message(int error_code, char *arg);