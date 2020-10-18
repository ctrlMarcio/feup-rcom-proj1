/**
 * @file    serial_port.h
 * @brief   Defines flags related to the serial port.
 * @date    2020-10-11
 */

#pragma once

/**
 * @brief States the beginning and end of a message.
 */
#define FRAME_FLAG 0x7e

/**
 * @brief States the address used on a request from the sender or answer from the receiver.
 */
#define ADDRESS_SENDER_RECEIVER 0x03

/**
 * @brief States the address used on a request from the receiver or answer from the sender.
 * 
 */
#define ADDRESS_RECEIVER_SENDER 0x01

/**
 * @brief States the control flag for the SET message.
 */
#define CONTROL_SET 0x03

/**
 * @brief States the control flag for the UA message.
 * 
 */
#define CONTROL_UA 0x07

/**
 * @brief States the control flag for the I message (one).
 * 
 */
#define CONTROL_I_ONE 0x40


/**
 * @brief States the control flag for the I message (zero).
 * 
 */
#define CONTROL_I_ZERO 0x00

#define CONTROL_RR_ONE 0x85

#define CONTROL_RR_ZERO 0x05

#define CONTROL_REJ_ONE 0x81

#define CONTROL_REJ_ZERO 0x01

#define MAX_FRAME_SIZE 102400 // 100 KByte

typedef struct MessageConstructor
{
    char address;
    char control;
    int data;
} MessageConstruct;