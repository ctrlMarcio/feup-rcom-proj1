/**
 * @file    error.h
 * @brief   Defines the error flags.
 */

#pragma once

/**
 * @brief Defines the error code for errors related to the arguments of the sender program.
 */
#define SENDER_ARGS_ERROR -1

/**
 * @brief Defines the error code for errors related to the arguments of the receiver program.
 */
#define RECEIVER_ARGS_ERROR -2

/**
 * @brief Defines the error code for errors related to opening and config ports.
 */
#define CONFIG_PORT_ERROR -3

/**
 * @brief Defines the error code for errors that occur while establishing the connection.
 */
#define ESTABLISH_CONNECTION_ERROR -4

/**
 * @brief Defines the error code for errors that occur while closing the connection.
 */
#define CLOSE_CONNECTION_ERROR -5

/**
 * @brief Defines the error called when a packet is lost.
 */
#define LOST_FRAME_ERROR -6

/**
 * @brief Defines the error called when the start packet is lost. 
 */
#define LOST_START_PACKET_ERROR -7

/**
 * @brief Defines the error called when the end packet is lost. 
 */
#define LOST_END_PACKET_ERROR -8

/**
 * @brief Defines the error called when a data packet is lost. 
 */
#define LOST_DATA_PACKET_ERROR -9

/**
 * @brief Defines the error called when a packet with an unexpected control type byte is received.
 */
#define INVALID_CTRL_PACKET_TYPE -10

/**
 * @brief Defines the errors related to the file handling.
 */
#define FILE_ERROR -11

/**
 * @brief Defines the error code that is called whenever the file to transfer already exists in the receiver end.
 */
#define EXISTING_FILE_ERROR -12

/**
 * @brief Defines the error that is called whenever a repeated frame is received in the receiver end.
 */
#define REPEATED_FRAME_ERROR -13

/**
 * @brief Prints a custom message for a given error code.
 * 
 * @param error_code    the error code.
 */
void print_error(int error_code);

/**
 * @brief Prints a custom message for a given error code whenever it can hold more information through a string.
 * 
 * @param error_code    the error code.
 * @param arg           the string that holds more information.
 */
void print_error_message(int error_code, char *arg);