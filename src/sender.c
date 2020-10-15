#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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
    define_set_frame(set_message);

    int connection_error = attempt_handshake(fd, set_message, NR_ATTEMPTS, TIMEOUT);
    if (!connection_error) printf("Connected\n");

    // define the information frame
    unsigned char message[255]; // FIXME
    unsigned char data[33] = "ola flavia carvalho ~~carvalhido";
    define_message_frame(message, 0, data);

    // send the information frame
    int sent_message_error = send_message(fd, message);
    printf("Sent error: %d\n", sent_message_error);

    if ((res = terminate_connection(fd, &oldtio)))
        return res;
}