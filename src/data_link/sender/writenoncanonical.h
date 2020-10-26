/**
 * @file    writenoncanonical.h
 * @brief   States important functions for non canonical writing.
 * @date    2020-10-11
 */

#pragma once

#include <termios.h>
#include <unistd.h>

#include "../common.h"

/**
 * @brief 
 *
 * @param fd 
 * @return int 
 */
int attempt_establishment(int fd);

/**
 * @brief Contructs the message being sent
 * 
 * @param message the built message as an array
 * @return int 0 if success, 1 otherwise
 */
int define_message_frame(char* message, char* data, int data_size, int sequence_number);

/**
 * @brief Sends the message
 * 
 * @param message the built message as an array
 * @return int
 */
int send_information_frame(int fd, char *message, int frame_size, int sequence_number);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 * @return int          the error code in case of error, 0 on success
 */
int terminate_sender_connection(int fd, struct termios *oldtio);

int send_disc_frame(int fd, char *disc_frame);