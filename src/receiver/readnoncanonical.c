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

int answer_sequence_number = 1;

void receive_set_frame(int fd);
void define_ua_frame(unsigned char* ua_frame);
void send_ua_frame(unsigned char* ua_frame, int fd);
void receive_information_frame(int fd);
void define_rr_frame(unsigned char* rr_frame);
void send_rr_frame(unsigned char* rr_frame, int fd);

void answer_establishment(int fd) {
    receive_set_frame(fd);

    unsigned char ua_frame[5];
    define_ua_frame(ua_frame);

    send_ua_frame(ua_frame, fd);
}

void answer_information(int fd) {
    receive_information_frame(fd);

    unsigned char rr_frame[5];
    define_rr_frame(rr_frame);

    send_rr_frame(rr_frame, fd);
    answer_sequence_number = (answer_sequence_number + 1) % 2;
}

void receive_set_frame(int fd) {
    unsigned char request_frame[5];
    char buf[255];

    MessageConstruct set = { .address = ADDRESS_SENDER_RECEIVER, .control = CONTROL_SET, .data = FALSE };
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

void define_ua_frame(unsigned char* ua_frame) {
    ua_frame[0] = FRAME_FLAG;
    ua_frame[1] = ADDRESS_SENDER_RECEIVER;
    ua_frame[2] = CONTROL_UA;
    ua_frame[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_UA);
    ua_frame[4] = FRAME_FLAG;
}

void send_ua_frame(unsigned char* ua_frame, int fd) {
    int res = write(fd, ua_frame, sizeof(unsigned char) * 5);
    printf("%d bytes sent\n", res);
}

void send_rr_frame(unsigned char* rr_frame, int fd) {
    int res = write(fd, rr_frame, sizeof(unsigned char) * 5);
    printf("%d bytes sent\n", res);
}

void terminate_receiver_connection(int fd, struct termios* oldtio) {
    tcsetattr(fd, TCSANOW, oldtio);
    close(fd);
}

void receive_information_frame(int fd) {
    unsigned char information_frame[MAX_FRAME_SIZE];
    char buf[1];

    char control_i;
    // Nr = 1 receives a Ns = 0
    if (answer_sequence_number)
        control_i = CONTROL_I_ZERO;
    else
        control_i = CONTROL_I_ONE;

    MessageConstruct set = { .address = ADDRESS_SENDER_RECEIVER, .control = control_i, .data = TRUE };
    enum set_state state = START;

    // TODO save data in an array

    unsigned int i = 0;
    while (state != STOP) {
        read(fd, buf, 1);

        if (buf[0] == ESCAPE) { // TODO improve destuff
            read(fd, buf, 1);

            information_frame[i] = buf[0];

            printf("%c\n", information_frame[i]);  // TEST
            i++;
            continue;
        }

        update_state(&state, buf[0], set);

        if (state == FLAG_RCV)
            i = 0;
        else if (state == STOP) {
            // TODO destuff
            // TODO check information_frame[i - 1] XOR
            // TODO send REJ if wrong
        }

        information_frame[i] = buf[0];

        printf("%c\n", information_frame[i]);  // TEST
        i++;
    }
}

void define_rr_frame(unsigned char* rr_frame) {
    rr_frame[0] = FRAME_FLAG;
    rr_frame[1] = ADDRESS_SENDER_RECEIVER;
    rr_frame[2] = CONTROL_RR_ZERO;
    if (answer_sequence_number) rr_frame[2] = CONTROL_RR_ONE;
    rr_frame[3] = XOR(rr_frame[1], rr_frame[2]);
    rr_frame[4] = FRAME_FLAG;
}