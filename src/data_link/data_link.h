/**
 * @file data_link.h
 * @brief Implements the main data link functions.
 */

#pragma once

#include "../util/util.h"

/**
 * @brief Establishes connection to the other end.
 * 
 * @param port      the port name (usually would be /dev/ttyS10).
 * @param entity    the entity (sender or receiver).
 * @return int      the fd of the port or negative in case of error.
 */
int llopen(char *port, enum entity entity);

/**
 * @brief Sends a buffer through the port.
 * 
 * @param fd        the fd of the port.
 * @param buffer    the buffer to send.
 * @param length    the size of the buffer.
 * @return int      the size of the sent packet or negative in case of error.
 */
int llwrite(int fd, char *buffer, int length);

/**
 * @brief Receives a buffer throught the port.
 * 
 * @param fd        the fd of the port.
 * @param buffer    the buffer to fill with the received information.
 * @return int      the size of the buffer read or negative in case of error.
 */
int llread(int fd, char *buffer);

/**
 * @brief Closes a connection to the other end.
 * 
 * @param fd        the fd of the port.
 * @return int      0 in case of success, else otherwise.
 */
int llclose(int fd);
