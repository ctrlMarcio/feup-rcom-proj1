/*Non-Canonical Input Processing*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "writenoncanonical.h"
#include "../error/error.h"
#include "../util/serial_port.h"
#include "../util/util.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

volatile int STOP = FALSE;

int count = 0, success = 0;

void alarm_handler() {  // atende alarme
    printf("alarme # %d\n", count);
    success = 0;
    count++;
}

int open_writing_serial_port(char *port, struct termios *oldtio) {
    struct termios newtio;

    int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
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

void define_set_packet(unsigned char *set_packet) {
    set_packet[0] = HANDSHAKE_FLAG;
    set_packet[1] = ADDRESS_SENDER_RECEIVER;
    set_packet[2] = CONTROL_SET;
    set_packet[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_SET);
    set_packet[4] = HANDSHAKE_FLAG;
}

int read_answer(int fd) {
    unsigned char answer_packet[5];
    success = 1;
    int i = 0;
    
    char buf[255] = {0};

    while (success) {               /* loop for input */
        int res = read(fd, buf, 1); /* returns after 1 char have been input */
        if (res < 0)
            continue;
        answer_packet[i] = buf[0];
        //printf("%X\n", buf[0]);

        if (i > 0 && answer_packet[0] && buf[0] == answer_packet[0])  // FIXME
        {
            success = 1;
            break;
        }
        i++;
    }

    return success;
}

int attempt_handshake(int fd, unsigned char *set_packet, int attempts, int timeout) {
    (void)signal(SIGALRM, alarm_handler);

    // receives the answer_packet
    while (count < timeout) {
        if (success)
            break;

        // sends the set
        int res = write(fd, set_packet, sizeof(unsigned char) * 5);
        alarm(timeout);
        printf("%d bytes sent\n", res);

        read_answer(fd);
    }

    return success;
}

int terminate_connection(int fd, struct termios *oldtio) {
    sleep(1);
    if (tcsetattr(fd, TCSANOW, oldtio) == -1) {
        perror("tcsetattr");
        return CONFIG_PORT_ERROR;
    }

    close(fd);
    return 0;
}