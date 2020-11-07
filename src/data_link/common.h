/**
 * @file common.h
 * @brief Defines functions that are important for both the sender and the receiver,
*/

#pragma once

#include "../util/util.h"
#include "util/serial_port.h"

/**
 * @brief Defines the timeout between attempts in seconds.
 */
#define TIMEOUT 5

/**
 * @brief Defines the number of attempts per message sent.
 */
#define NR_ATTEMPTS 5

/**
 * @brief Sends a frame that expects a valid answer, resending if the answer was invalid.
 * 
 * @param fd                    the fd of the port to send the frame to.
 * @param frame                 the frame to send.
 * @param frame_size            the size of the frame to send.
 * @param type                  the type of the frame to send (for output perposes).
 * @param answer_type           the expected answer type.
 * @param sender_to_receiver    the boolean that states if its the sender sending the frame.
 * @return int                  0 on success, else otherwise.
 */
int send_retransmission_frame(int fd, char* frame, unsigned frame_size, char* type, enum frame answer_type, bool sender_to_receiver);

/**
 * @brief Sends a frame that does not expect any answer.
 * 
 * @param fd                    the fd of the port to send the frame to.
 * @param frame                 the frame to send.
 * @param frame_size            the size of the frame to send.
 * @param type                  the type of the frame to send (for output perposes).
 * @return int                  0 on success, else otherwise.
 */
int send_unanswered_frame(int fd, char* frame, unsigned frame_size, char* type);

/**
 * @brief Receives a frame.
 * 
 * @param fd                    the fd of the port to receive the frame from.
 * @param frame_type            the frame type to receive.
 * @param sender_to_receiver    the boolean that states if the frame comes from the sender.
 * @param expect_rej            the boolean that states if the frame can be a REJ instead.
 * @return int                  0 on success, else otherwise.
 */
int receive_frame(int fd, enum frame frame_type, bool sender_to_receiver, bool expect_rej);

/**
 * @brief Receives a data frame.
 * 
 * @param fd                    the fd of the port to receive the frame from.
 * @param sequence_number       the sequence number to receiver.
 * @param buffer                the buffer to fill with the data.
 * @return int                  the size of the data (buffer) or negative in case of error.
 */
int receive_data_frame(int fd, int sequence_number, char* buffer);

/**
 * @brief Defines and builds an UA frame.
 * 
 * @param ua_frame              the frame to build.
 * @param sender_to_receiver    the boolean that defines if the frame will be send by the sender.
 */
void define_ua_frame(char* ua_frame, int sender_to_receiver);

/**
 * @brief The alarm handler, increases the count of a global variable.
 */
void alarm_handler();

/**
 * @brief Subscribes the alarm interruptions to call the alarm_handler.
 */
void subscribe_alarm_interruptions();
