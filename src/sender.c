#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "sender/writenoncanonical.h"
#include "util/util.h"

/**
 * @brief Defines the timeout between attempts in seconds.
 */
#define TIMEOUT 3

/**
 * @brief Defines the number of attempts per message sent.
 */
#define NR_ATTEMPTS 3

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    struct termios oldtio;
    int fd = open_writing_serial_port(argv[1], &oldtio);

    unsigned char set_message[5];
    define_set_packet(set_message);
    printf("%X %X %X %X %X\n", set_message[0], set_message[1], set_message[2], set_message[3], set_message[4]);

    int connected = attempt_handshake(fd, set_message, NR_ATTEMPTS, TIMEOUT);
    printf("\n\n%d\n\n", connected);

    if ((res = terminate_connection(fd, &oldtio)))
        return res;
}