#pragma once

/* IO calls */

#define CONNECTION_ESTABLISHED "Connection established"

#define FILE_SENT "File sent"

#define FILE_RECEIVED "File received"

/* Control packet flags and info */

#define FILE_SIZE_TYPE 0

#define FILE_NAME_TYPE 1

#define DATA_SIZE 4096 // in bytes

/* Command line interface */

#define VIRTUAL_PORTS_FLAG "-v"

#define VIRTUAL_PORTS_EXTENDED_FLAG "--virtual"

/* Connection ports */

#define DEFAULT_PORT "/dev/ttyS0"

#define VIRTUAL_SENDER_PORT "/dev/ttyS10"

#define VIRTUAL_RECEIVER_PORT "/dev/ttyS11"