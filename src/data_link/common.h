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
 *
 */
int send_retransmission_frame(int fd, char* frame, unsigned frame_size, char* type, enum frame answer_type, bool sender_to_receiver);

/**
 *
 */
int send_unanswered_frame(int fd, char* frame, unsigned frame_size, char* type);

/**
 *
 */
int receive_frame(int fd, unsigned size, enum frame frame_type, bool sender_to_receiver, bool expect_rej);

/**
 * 
 */
int receive_data_frame(int fd, int sequence_number, char* buffer);

/**
 * 
 */
void define_ua_frame(char* ua_frame, int sender_to_receiver);

/**
 * 
 */
void alarm_handler();


