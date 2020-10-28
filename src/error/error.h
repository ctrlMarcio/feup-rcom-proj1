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

/**
 * @brief //TODO
 */
#define ESTABLISH_CONNECTION_ERROR -3

/**
 * @brief //TODO
 * 
 */
#define CLOSE_CONNECTION_ERROR -4

/**
 * @brief //TODO
 * 
 */
#define LOST_FRAME_ERROR -5

#define LOST_START_PACKET_ERROR -6

#define LOST_DATA_PACKET -7

#define INVALID_CTRL_PACKET_TYPE -8
