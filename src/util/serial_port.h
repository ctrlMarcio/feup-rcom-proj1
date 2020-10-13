/**
 * @file    serial_port.h
 * @brief   Defines flags related to the serial port.
 * @date    2020-10-11
 */

#pragma once

/**
 * @brief States the beginning and end of an handshake type of message, i.e. SET and UA messages.
 */
#define HANDSHAKE_FLAG 0x7e

/**
 * @brief States the address used on a request from the sender or answer from the receiver.
 */
#define ADDRESS_SENDER_RECEIVER 0X03

/**
 * @brief States the address used on a request from the receiver or answer from the sender.
 * 
 */
#define ADDRESS_RECEIVER_SENDER 0X01

/**
 * @brief States the control flag for the SET message.
 */
#define CONTROL_SET 0X03

/**
 * @brief States the control flag for the UA message.
 * 
 */
#define CONTROL_UA 0X07

typedef struct MessageConstructor {
    char flag;
    char address;
    char control;
} MessageConstruct;