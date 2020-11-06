/**
 * @file    writenoncanonical.h
 * @brief   States important functions for non canonical writing at data link level.
 */

#pragma once

#include <termios.h>
#include <unistd.h>

#include "../common.h"

/**
 * @brief       Establishes connection to the receiver sending a SET and receiving an UA.
 *
 * @param fd    the fd of the port.
 * @return int  0 in case of success, otherwise in case of error.
 */
int attempt_establishment(int fd);

/**
 * @brief Builds an information frame.
 * 
 * @param message           the message to build.
 * @param data              the data to send.
 * @param data_size         the size of the data to send.
 * @param sequence_nubmer   the sender sequence number.
 * @return int              0 in case of success, otherwise in case of error.
 */
int define_message_frame(char* message, char* data, int data_size, int sequence_number);

/**
 * @brief Sends an information frame. Covering errors.
 * 
 * @param fd                the fd of the port.
 * @param message           the frame to send.
 * @param frame_size        the size of the frame to send.
 * @param sequence_nubmer   the sender sequence number.
 * @return int              0 in case of success, otherwise in case of error.
 */
int send_information_frame(int fd, char *message, int frame_size, int sequence_number);

/**
 * @brief Reposes the termios and closes the port fd.
 * 
 * @param fd                the fd of the port.
 * @param oldtio            the old termios structure to reset
 * @return int              0 in case of success, the error code in case of error
 */
int terminate_sender_connection(int fd, struct termios *oldtio);

/**
 * @brief Sends a DISC frame.
 * 
 * @param fd                the fd of the port.
 * @param disc_frame        the disc frame to send.
 * @return int              0 in case of success, otherwise in case of error.
 */
int send_disc_frame(int fd, char *disc_frame);