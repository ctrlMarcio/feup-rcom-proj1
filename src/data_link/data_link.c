#include "data_link.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "../error/error.h"
#include "receiver/readnoncanonical.h"
#include "sender/writenoncanonical.h"
#include "util/serial_port.h"
#include "util/state_machine.h"

#define BAUDRATE B38400

struct termios oldtio;

int open_serial_port(char* port);
void define_disc_frame(unsigned char* disc_frame, bool sender_to_receiver);
int close_sender(int fd);
int close_receiver(int fd);
void receive_disc_frame(int fd, bool sender);

int llopen(char* port, bool sender) {
    int fd = open_serial_port(port);
    if (fd < 0)
        return fd;

    if (sender) {
        if (attempt_establishment(fd))
            return ESTABLISHMENT_ERROR;
    }
    else {
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
        return close_receiver(fd);
}

int close_sender(int fd) {
    unsigned char disc_frame[5];
    define_disc_frame(disc_frame, TRUE);

    send_retransmission_frame(fd, disc_frame, 5, "DISC", DISC, TRUE, 0);

    unsigned char ua_frame[5];
    define_ua_frame(ua_frame, FALSE);

    send_unanswered_frame(fd, ua_frame, 5, "UA");

    return terminate_sender_connection(fd, &oldtio);
}

int close_receiver(int fd) {
    receive_frame(fd, 5, DISC, TRUE, 0, FALSE); // TODO sequence number not required

    unsigned char disc_frame[5];
    define_disc_frame(disc_frame, FALSE);

    if (send_retransmission_frame(fd, disc_frame, 5, "DISC", UA, FALSE, 1)) // TODO sequence number not required
        printf("coiso\n");  // TODO error

    return terminate_sender_connection(fd, &oldtio);
}

void define_disc_frame(unsigned char* disc_frame, bool sender_to_receiver) {
    disc_frame[0] = FRAME_FLAG;
    if (sender_to_receiver)
        disc_frame[1] = ADDRESS_SENDER_RECEIVER;
    else
        disc_frame[1] = ADDRESS_RECEIVER_SENDER;
    disc_frame[2] = CONTROL_DISC;
    disc_frame[3] = XOR(disc_frame[1], disc_frame[2]);
    disc_frame[4] = FRAME_FLAG;
}

void receive_disc_frame(int fd, bool sender) {
    unsigned char disc_frame[5];
    char buf[1];

    unsigned char address;
    if (sender)
        address = ADDRESS_RECEIVER_SENDER;
    else
        address = ADDRESS_SENDER_RECEIVER;

    MessageConstruct disc = { .address = address, .control = CONTROL_DISC, .data = FALSE };

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
}

/* 🥰🧡💛💚💙💜🤎🖤🤍💕💞💓💗💖💘💝 :((( */