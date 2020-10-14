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

    int connected = attempt_handshake(fd, set_message, NR_ATTEMPTS, TIMEOUT);
    if(connected) printf("Connected\n");

    //actually definir a mensagem
    unsigned char message[9];
    unsigned char *data="ola flavia carvalho carvalhido~";
    define_message_packet(message,1,data);

    //actually mandar a mensagem
    int sent = send_message(message);

    if ((res = terminate_connection(fd, &oldtio)))
        return res;
}