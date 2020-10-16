/*Non-Canonical Input Processing*/

#include "readnoncanonical.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "../error/error.h"
#include "../util/serial_port.h"
#include "../util/state_machine.h"
#include "../util/util.h"

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

void receive_set_frame(int fd);
void define_ua_frame(unsigned char *ua_frame);
void send_ua_frame(unsigned char *ua_frame, int fd);

void answer_establishment(int fd) {
    receive_set_frame(fd);

    unsigned char ua_frame[5];
    define_ua_frame(ua_frame);

    send_ua_frame(ua_frame, fd);
}

void receive_set_frame(int fd) {
    unsigned char request_frame[5];
    char buf[255];

    MessageConstruct set = {.address = ADDRESS_SENDER_RECEIVER, .control = CONTROL_SET, .data = FALSE};
    enum set_state state = START;

    unsigned int i = 0;
    while (state != STOP) {
        int res = read(fd, buf, 1);
        if (res < 0)
            continue;
            
        request_frame[i] = buf[0];
        update_state(&state, request_frame[i], set);
        i++;
    }
}

void define_ua_frame(unsigned char *ua_frame) {
    ua_frame[0] = FRAME_FLAG;
    ua_frame[1] = ADDRESS_SENDER_RECEIVER;
    ua_frame[2] = CONTROL_UA;
    ua_frame[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_UA);
    ua_frame[4] = FRAME_FLAG;
}

void send_ua_frame(unsigned char *ua_frame, int fd) {
    int res = write(fd, ua_frame, sizeof(unsigned char) * 5);
    printf("%d bytes sent\n", res);
}

void terminate_receiver_connection(int fd, struct termios *oldtio) {
    tcsetattr(fd, TCSANOW, oldtio);
    close(fd);
}

void receive_message(int fd) {
    int control_setter = 0;

    //unsigned char data[1000];

    while (1) {
        receive_frame(fd, control_setter);
        if (control_setter) control_setter = 0;
        else control_setter = 1;
    }
}

void receive_frame(int fd, int control_setter) {
    unsigned char information_frame[255];  // FIXME
    char buf[255];

    char control_i;
    if (control_setter)
        control_i = CONTROL_I_ONE;
    else
        control_i = CONTROL_I_ZERO;

    MessageConstruct set = {.address = ADDRESS_SENDER_RECEIVER, .control = control_i, .data = TRUE, .data_received = 0};
    enum set_state state = START;

    unsigned int i = 0;
    while (state != STOP) {
        read(fd, buf, 1);
        information_frame[i] = buf[0];
        printf("%c\n", information_frame[i]);  // TEST
        update_state(&state, information_frame[i], set);
        if (state == DATA)
            set.data_received += 1;
        i++;
    }
}