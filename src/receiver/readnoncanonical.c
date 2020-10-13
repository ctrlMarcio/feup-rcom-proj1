/*Non-Canonical Input Processing*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "readnoncanonical.h"
#include "../util/serial_port.h"
#include "../util/util.h"
#include "../util/state_machine.h"
#include "../error/error.h"

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

int open_reading_serial_port(char *port, struct termios *oldtio) {
    struct termios newtio;

    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror(port);
        exit(CONFIG_PORT_ERROR);
    }

    if (tcgetattr(fd, oldtio) == -1) { /* save current port settings */
        perror("tcgetattr");
        exit(CONFIG_PORT_ERROR);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;  /* blocking read until 5 chars received */

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) == -1) {
        perror("tcsetattr");
        exit(CONFIG_PORT_ERROR);
    }

    printf("New termios structure set\n");
    return fd;
}

void read_set_packet(int fd) {
    unsigned char request_packet[5];
    char buf[255];

    MessageConstruct set = { .flag = HANDSHAKE_FLAG, .address = ADDRESS_SENDER_RECEIVER, .control = CONTROL_SET};
    enum set_state state = START;

    unsigned int i = 0;
    while (state != STOP) {    
        read(fd, buf, 1);
        request_packet[i] = buf[0];
        printf("%X\n", request_packet[i]); // TEST
        update_state(&state, request_packet[i], set);
        i++;
    }
}

void define_ua_packet(unsigned char *ua_packet) {
    ua_packet[0] = HANDSHAKE_FLAG;
    ua_packet[1] = ADDRESS_SENDER_RECEIVER;
    ua_packet[2] = CONTROL_UA;
    ua_packet[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_UA);
    ua_packet[4] = HANDSHAKE_FLAG;
}

void send_ua_packet(unsigned char *ua_packet, int fd) {
    int res = write(fd, ua_packet, sizeof(unsigned char) * 5);
    printf("%d bytes sent\n", res);
}

void terminate_connection(int fd, struct termios *oldtio) {
    tcsetattr(fd, TCSANOW, oldtio);
    close(fd);
}