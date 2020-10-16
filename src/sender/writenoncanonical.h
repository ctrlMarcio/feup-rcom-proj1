/**
 * @file    writenoncanonical.h
 * @brief   States important functions for non canonical writing.
 * @date    2020-10-11
 */

#pragma once

#include <termios.h>
#include <unistd.h>

/**
 * @brief Defines the timeout between attempts in seconds.
 */
#define TIMEOUT 3

/**
 * @brief Defines the number of attempts per message sent.
 */
#define NR_ATTEMPTS 3

/**
 * @brief 
 * // TODO
 * @param fd 
 * @return int 
 */
int attempt_establishment(int fd);

/**
 * @brief Contructs the message being sent
 * 
 * @param message the built message as an array
 * @return int 1 if success, 0 otherwise
 */
int define_message_frame(unsigned char *message, int control_setter, unsigned char* data);

/**
 * @brief Sends the message
 * 
 * @param fd        // TODO
 * @param message the built message as an array
 * @return int
 */
int send_message(int fd, unsigned char *message);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 * @return int          the error code in case of error, 0 on success
 */
int terminate_sender_connection(int fd, struct termios *oldtio);

/**
 * @brief 
 * 
 * @param fd
 * @deprecated 
 */
void receive_message(int fd);

/**
 * @brief 
 * 
 * // TODO
 * 
 * @param fd 
 * @param control_setter 
 */
void receive_frame(int fd, int control_setter);