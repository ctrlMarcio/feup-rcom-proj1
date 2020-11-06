/**
 * @file readnoncanonical.h
 * @brief Stats important functions for non canonical reading at data link level.
 */

#pragma once

#include <termios.h>
#include <unistd.h>

#include "../common.h"

/**
 * @brief Establishes connection as the receiver, reading a SET and sending an UA.
 *
 * @param fd    the fd of the port.
 */
void answer_establishment(int fd);

/**
 * @brief Reads an information frame and sends the appropriate response.
 * 
 * @param fd        the fd of the port.
 * @param buffer    the buffer to read the data to.
 * @return int      the size of the data, negative in case of error.
 */
int answer_information(int fd, char *buffer);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 */
void terminate_receiver_connection(int fd, struct termios *oldtio);