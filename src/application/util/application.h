/**
 * @file application.h
 * @brief Application protocol and interface macros
 */

#pragma once

/* Application protocol */

/**
 * @brief Control packet file size type indication.
 */
#define FILE_SIZE_TYPE 0

/**
 * @brief Control packet file name type indication.
 */
#define FILE_NAME_TYPE 1

/* Application packet */

/**
 * @brief Data packet starting byte.
 */
#define CONTROL_DATA 1

/**
 * @brief Start control packet starting byte.
 */
#define CONTROL_START 2

/**
 * @brief End control packet starting byte.
 */
#define CONTROL_END 3

/* IO calls */

/**
 * @brief The string that states that the connection is established.
 */
#define CONNECTION_ESTABLISHED  "Connection established"

/**
 * @brief The string that states that the file was sent.
 */
#define FILE_SENT               "File sent"

/**
 * @brief The string that states that the file was receiver.
 */
#define FILE_RECEIVED           "File received"

/* Command line interface */

/**
 * @brief Command line interface argument calling for virtual ports.
 */
#define VIRTUAL_PORTS_FLAG          "-v"

/**
 * @brief Alternate command line interface argument calling for virtual ports.
 */
#define VIRTUAL_PORTS_EXTENDED_FLAG "--virtual"

/* Connection ports */

/**
 * @brief The default port.
 */
#define DEFAULT_PORT            "/dev/ttyS0"

/**
 * @brief The default sender virtual port.
 */
#define VIRTUAL_SENDER_PORT     "/dev/ttyS10"

/**
 * @brief The default receiver virtual port.
 */
#define VIRTUAL_RECEIVER_PORT   "/dev/ttyS11"

/* Size constants */

/**
 * @brief The size of the data that sends in a packet.
 */
#define DATA_SIZE           512 // in bytes

/**
 * @brief The max allowed file name size.
 */
#define MAX_FILE_NAME_SIZE  1024 // in bytes

/**
 * @brief The max allowed user input size.
 */
#define MAX_USER_INPUT_SIZE 1024 