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

#include "../../error/error.h"
#include "../../util/util.h"
#include "../util/serial_port.h"
#include "../util/state_machine.h"

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int answer_sequence_number = 1;

void receive_set_frame(int fd);
void receive_information_frame(int fd);
void define_rr_frame(char* rr_frame);
void send_rr_frame(char* rr_frame, int fd);
void send_rej_frame(int fd);
void define_rej_frame(char* rej_frame);

void answer_establishment(int fd) {
    receive_set_frame(fd);
    char ua_frame[5];
    define_ua_frame(ua_frame, TRUE);

    send_unanswered_frame(fd, ua_frame, 5, "UA");
}

int answer_information(int fd, char* buffer) {
    int data_size;
    while ((data_size = receive_data_frame(fd, !answer_sequence_number, buffer)) < 0);

    char rr_frame[5];
    define_rr_frame(rr_frame);

    if (send_unanswered_frame(fd, rr_frame, 5, "RR"))
        return LOST_FRAME_ERROR;

    answer_sequence_number = 1 - answer_sequence_number; // alternate between 0 and 1
    return data_size;
}

void terminate_receiver_connection(int fd, struct termios* oldtio) {
    tcsetattr(fd, TCSANOW, oldtio);
    close(fd);
}

/*******************************************************
 *                  PRIVATE FUNCTIONS                  *
********************************************************/

void receive_set_frame(int fd) {
    char request_frame[5];
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

void send_rr_frame(char* rr_frame, int fd) {
    int res = write(fd, rr_frame, sizeof(char) * 5);
    if (OUTPUT) printf("%d bytes sent in a RR frame\n", res);
}

void send_rej_frame(int fd) {
    char rej_frame[5];
    define_rej_frame(rej_frame);

    int res = write(fd, rej_frame, sizeof(char) * 5);
    if (OUTPUT) printf("%d bytes sent in a REJ frame\n", res);
}

void define_rr_frame(char* rr_frame) {
    rr_frame[0] = FRAME_FLAG;
    rr_frame[1] = ADDRESS_SENDER_RECEIVER;
    rr_frame[2] = CONTROL_RR_ZERO;
    if (answer_sequence_number) rr_frame[2] = CONTROL_RR_ONE;
    rr_frame[3] = XOR(rr_frame[1], rr_frame[2]);
    rr_frame[4] = FRAME_FLAG;
}