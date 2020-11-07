/**
 * @file    serial_port.h
 * @brief   Defines flags related to the data link.
 */

#pragma once

/**
 * @brief The flag that indicates if the print of frame transference should be present.
 */
#define OUTPUT 0

/**
 * @brief The virtual bit error ratio, 0 for turned off. 1/BER percentage for BER > 0.
 */
#define BER 0 

/**
 * @brief The virtual propagation time in milliseconds.
 */
#define TPROP 0

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
 * @brief States the control flag for the DISC message.
 */
#define CONTROL_DISC 0x0b

/**
 * @brief States the control flag for the I message with the sequence number as 1.
 */
#define CONTROL_I_ONE 0x40


/**
 * @brief States the control flag for the I message with the sequence number as 0.
 */
#define CONTROL_I_ZERO 0x00

/**
 * @brief States the control flag for the RR message with the sequence number as 1.
 */
#define CONTROL_RR_ONE 0x85

/**
 * @brief States the control flag for the RR message with the sequence number as 0.
 */
#define CONTROL_RR_ZERO 0x05

/**
 * @brief States the control flag for the REJ message with the sequence number as 1.
 */
#define CONTROL_REJ_ONE 0x81

/**
 * @brief States the control flag for the REJ message with the sequence number as 0.
 */
#define CONTROL_REJ_ZERO 0x01

/**
 * @brief The flag that XORs with the byte to stuff.
 */
#define STUFF_FLAG 0x20

/**
 * @brief The max size of a frame in bytes.
 */
#define MAX_FRAME_SIZE 102400 // 100 KBs

/**
 * @brief The max size of a packet in bytes.
 */
#define MAX_PACKET_SIZE (MAX_FRAME_SIZE + 4) // 100 KBs

/**
 * @brief The possible frame types.
 */
enum frame{NONE, UA, SET, I, RR, REJ, DISC};

/**
 * @brief The required fields of a message.
 */
typedef struct MessageConstructor {
    /**
     * @brief The messsage address.
     */
    char address;

    /**
     * @brief The message control.
     */
    char control;

    /**
     * @brief The control of the inverse sequence number of the message.
     */
    char inverse_control;

    /**
     * @brief The boolean that states if frame brings data.
     */
    int data;
} MessageConstruct;

/**
 * @brief Gets the control byte, given the frame type and the sequence number.
 * 
 * @param type              the frame type.
 * @param sequence_number   the sequence number.
 * @return char             the control byte.
 */
char get_control(enum frame type, int sequence_number);