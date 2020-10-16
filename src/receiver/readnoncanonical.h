/**
 * @file readnoncanonical.h
 * @brief 
 * @date 2020-10-11
 */

#pragma once

#include <termios.h>
#include <unistd.h>

/**
 * @brief 
 * // TODO
 * @param fd 
 */
void answer_establishment(int fd);

void answer_information(int fd);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 */
void terminate_receiver_connection(int fd, struct termios *oldtio);