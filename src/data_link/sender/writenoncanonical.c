/*Non-Canonical Input Processing*/
#include "writenoncanonical.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../error/error.h"
#include "../../util/util.h"
#include "../util/serial_port.h"
#include "../util/state_machine.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int count = 0, success = 0;

void define_set_frame(char* set_frame);
int read_ua_answer(int fd);
int read_receiver_answer(int fd, int sequence_number);
int connect_to_receiver(int fd, char* set_frame);

int attempt_establishment(int fd) {
    char set_frame[5];
    define_set_frame(set_frame);

    return send_retransmission_frame(fd, set_frame, 5, "SET", UA, TRUE, 0); // TODO dont need for sequence number
}

int define_message_frame(char* message, char* data, int data_size, int sequence_number) {
    message[0] = FRAME_FLAG;
    message[1] = ADDRESS_SENDER_RECEIVER;
    if (sequence_number)
        message[2] = CONTROL_I_ONE;
    else
        message[2] = CONTROL_I_ZERO;
    message[3] = XOR(message[1], message[2]);

    for (unsigned int i = 0; i < data_size; i++)
        message[i + 4] = data[i];

    message[data_size + 4] = FRAME_FLAG;

    return 0;
}

int send_information_frame(int fd, char* message, int frame_size, int sequence_number) {
    (void)signal(SIGALRM, alarm_handler);

    count = 0, success = 0;

    // receives the answer_frame
    while (count < NR_ATTEMPTS) {
        if (success)
            break;

        // sends the frame
        int res = write(fd, message, sizeof(char) * frame_size);
        alarm(TIMEOUT);
        printf("%d bytes sent in an I frame\n", res);

        read_receiver_answer(fd, sequence_number);
    }

    if (success)
        sequence_number = 1 - sequence_number;

    return !success;
}

int send_disc_frame(int fd, char *disc_frame) {
    (void)signal(SIGALRM, alarm_handler);

    count = 0, success = 0;

    // receives the answer_frame
    while (count < NR_ATTEMPTS) {
        if (success)
            break;

        // sends the disc
        int res = write(fd, disc_frame, sizeof(char) * 5);
        alarm(TIMEOUT);
        printf("%d bytes sent in a DISC frame\n", res);

        success = TRUE;
        // receive_disc_frame(fd, TRUE); // TEST
    }

    return !success;
}

int terminate_sender_connection(int fd, struct termios* oldtio) {
    sleep(1);
    if (tcsetattr(fd, TCSANOW, oldtio) == -1) {
        perror("tcsetattr");
        return CONFIG_PORT_ERROR;
    }

    close(fd);
    return 0;
}

void define_set_frame(char* set_frame) {
    set_frame[0] = FRAME_FLAG;
    set_frame[1] = ADDRESS_SENDER_RECEIVER;
    set_frame[2] = CONTROL_SET;
    set_frame[3] = XOR(set_frame[1], set_frame[2]);
    set_frame[4] = FRAME_FLAG;
}

int read_ua_answer(int fd) {
    char answer_frame[5];
    success = 1;
    int i = 0;

    MessageConstruct ua = {.address = ADDRESS_SENDER_RECEIVER, .control = CONTROL_UA, .data = FALSE};
    enum set_state rr_state = START;

    char buf[255] = {0};

    while (success) {
        int res = read(fd, buf, 1);
        if (res < 0)
            continue;
        answer_frame[i] = buf[0];
        update_state(&rr_state, answer_frame[i], ua);

        if (rr_state == STOP) {
            success = 1;
            break;
        }
        i++;
    }

    return success;
}

int read_receiver_answer(int fd, int sequence_number) {
    success = 1;
    int i = 0;

    char control_rr = CONTROL_RR_ONE;
    if (sequence_number) control_rr = CONTROL_RR_ZERO;

    MessageConstruct rr = {.address = ADDRESS_SENDER_RECEIVER, .control = control_rr, .data = FALSE};
    enum set_state rr_state = START;

    char control_rej = CONTROL_REJ_ONE;
    if (sequence_number) control_rej = CONTROL_REJ_ZERO;

    MessageConstruct rej = {.address = ADDRESS_RECEIVER_SENDER, .control = control_rej, .data = FALSE};
    enum set_state rej_state = START;

    char buf[1] = {0};

    while (success) {
        int res = read(fd, buf, 1);
        if (res < 0)
            continue;
        update_state(&rr_state, buf[0], rr);
        update_state(&rej_state, buf[0], rej);

        if (rr_state == FLAG_RCV && rej_state == FLAG_RCV)
            i = 0;
        else if (rr_state == STOP) {
            success = 1;
            break;
        } else if (rej_state == STOP) {  // if it reads a REJ frame, returns unsuccess
            alarm(0);                    // anticipates alarm
            alarm_handler();
            break;
        }

        i++;
    }

    return success;
}

int connect_to_receiver(int fd, char* set_frame) {
    (void)signal(SIGALRM, alarm_handler);

    // receives the answer_frame
    while (count < NR_ATTEMPTS) {
        if (success)
            break;

        // sends the set
        int res = write(fd, set_frame, sizeof(char) * 5);
        alarm(TIMEOUT);
        printf("%d bytes sent in a SET frame\n", res);

        read_ua_answer(fd);
    }

    return !success;
}