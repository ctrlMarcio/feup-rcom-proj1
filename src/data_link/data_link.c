#include "data_link.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "error/error.h"
#include "receiver/readnoncanonical.h"
#include "sender/writenoncanonical.h"
#include "util/serial_port.h"
#include "util/state_machine.h"

#define BAUDRATE B38400

struct termios oldtio;

int open_serial_port(char* port);
int attempt_interruption(int fd);
void define_disc_frame(unsigned char* disc_frame);
void receive_disc_frame(int fd, bool sender);
void define_ua_frame(unsigned char* ua_frame);
void send_ua_frame(unsigned char* ua_frame, int fd);
int close_sender(int fd);

int llopen(char* port, bool sender) {
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

int open_serial_port(char* port) {
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

int llclose(int fd, bool sender) {
    if (sender)
        return close_sender(fd);
    else 
        
        // TODO receive disc

        // TODO send dsic

        // terminate connection
}

int close_sender(int fd) {
    int interruption = attempt_interruption(fd);
    if (interruption)
        printf("coiso\n");  // TODO error

    receive_disc_frame(fd, TRUE);

    unsigned char ua_frame[5];
    define_ua_frame(ua_frame);

    send_ua_frame(ua_frame, fd);

    // TODO terminate connection

    return 0;
}

int attempt_interruption(int fd) {
    unsigned char disc_frame[5];
    define_disc_frame(disc_frame);

    return send_disc_frame(fd, disc_frame);
}

void define_disc_frame(unsigned char* disc_frame) {
    disc_frame[0] = FRAME_FLAG;
    disc_frame[1] = ADDRESS_SENDER_RECEIVER;
    disc_frame[2] = CONTROL_DISC;
    disc_frame[3] = XOR(disc_frame[1], disc_frame[2]);
    disc_frame[4] = FRAME_FLAG;
}

void receive_disc_frame(int fd, bool sender) {
    unsigned char disc_frame[5];
    char buf[1];

    unsigned char address;
    if (sender)
        address = ADDRESS_SENDER_RECEIVER;
    else
        address = ADDRESS_RECEIVER_SENDER;

    MessageConstruct disc = {.address = address, .control = CONTROL_DISC, .data = FALSE};

    enum set_state state = START;

    unsigned int i = 0;
    while (state != STOP) {
        int res = read(fd, buf, 1);
        if (res < 0)
            continue;

        disc_frame[i] = buf[0];
        update_state(&state, disc_frame[i], disc);
        i++;
    }

    // 🥰🧡💛💚💙💜🤎🖤🤍💕💞💓💗💖💘💝 :(((
}

void define_ua_frame(unsigned char* ua_frame) {
    ua_frame[0] = FRAME_FLAG;
    ua_frame[1] = ADDRESS_SENDER_RECEIVER;
    ua_frame[2] = CONTROL_UA;
    ua_frame[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_UA);
    ua_frame[4] = FRAME_FLAG;
}

void send_ua_frame(unsigned char* ua_frame, int fd) {
    int res = write(fd, ua_frame, sizeof(unsigned char) * 5);
    printf("%d bytes sent in an UA frame\n", res);
}