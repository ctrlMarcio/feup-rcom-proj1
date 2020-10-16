#include "data_link.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "error/error.h"
#include "sender/writenoncanonical.h"
#include "receiver/readnoncanonical.h"

#define BAUDRATE B38400

struct termios oldtio;

int open_serial_port(char *port);

int llopen(char *port, bool sender) {
    int fd = open_serial_port(port);
    if (fd < 0)
        return fd;

    if (sender) {
        if (attempt_establishment(fd))
            return ESTABLISHMENT_ERROR;
    } else {
        answer_establishment(fd);
    }

    return fd;
}

int open_serial_port(char *port) {
    struct termios newtio;

    int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        perror(port);
        return CONFIG_PORT_ERROR;
    }

    if (tcgetattr(fd, &oldtio) == -1) { /* save current port settings */
        perror("tcgetattr");
        return CONFIG_PORT_ERROR;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) == -1) {
        perror("tcsetattr");
        return CONFIG_PORT_ERROR;
    }

    printf("New termios structure set\n");
    return fd;
}