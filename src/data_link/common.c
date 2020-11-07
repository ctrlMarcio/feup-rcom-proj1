#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#include "util/state_machine.h"
#include "../error/error.h"

int count, success;
char last_frame[MAX_FRAME_SIZE];
int last_frame_size = 0;

void define_rej_frame(char* rej_frame, int sequence_number);
int parse_data(int data_size, char* data_array, int fd, int i, char* frame, int sequence_number);
void apply_errors(char* buf);
bool verify_repeated(bool sequence_correct, char* frame, int i);

int send_retransmission_frame(int fd, char* frame, unsigned frame_size, char* type, enum frame answer_type, bool sender_to_receiver) {
    count = 0, success = 0;

    while (count < NR_ATTEMPTS) {
        if (success)
            break;

        int res = write(fd, frame, sizeof(char) * frame_size);
        alarm(TIMEOUT);
        if (OUTPUT) printf("%d bytes sent in a %s frame\n", res, type);

        if (answer_type == UA)
            receive_frame(fd, UA, sender_to_receiver, FALSE);
        else if (answer_type == DISC)
            receive_frame(fd, DISC, !sender_to_receiver, FALSE); // !sender_to_receiver bc DISC is not an answer, is a new request
    }

    return !success;
}

int send_unanswered_frame(int fd, char* frame, unsigned frame_size, char* type) {
    int res = write(fd, frame, sizeof(char) * frame_size);
    if (res < 0)
        return 1;

    if (OUTPUT) printf("%d bytes sent in a %s frame\n", res, type);
    return 0;
}

int receive_frame(int fd, enum frame frame_type, bool sender_to_receiver, bool expect_rej) {
    if (frame_type == I) // compatibility ig
        return 1;

    success = 1;

    // define message construct
    char address;
    if (sender_to_receiver)
        address = ADDRESS_SENDER_RECEIVER;
    else
        address = ADDRESS_RECEIVER_SENDER;
    char control = get_control(frame_type, 0);
    bool data = (frame_type == I);
    MessageConstruct construct = { .address = address, .control = control, .data = data };
    enum set_state state = START;

    // define REJ message construct in case REJ frame is waited
    control = get_control(REJ, 0);
    MessageConstruct rej = { .address = ADDRESS_RECEIVER_SENDER, .control = control, .data = FALSE };
    enum set_state rej_state = START;

    char buf[255] = { 0 };

    unsigned int i = 0;
    while (success) {
        // read from the port
        int res = read(fd, buf, 1);
        if (res < 0) continue;

        // update machine states
        update_state(&state, buf[0], construct);

        if (expect_rej)
            update_state(&rej_state, buf[0], rej);

        // reset if finds flag for the first time
        if (state == FLAG_RCV && (!expect_rej || rej_state == FLAG_RCV))
            i = 0;
        else if (state == STOP) { // terminate if reads all
            alarm(0);
            success = TRUE;
            break;
        }

        else if (rej_state == STOP) { // if it reads a REJ frame, returns unsuccess
            alarm(0);                   // anticipates alarm
            alarm_handler();
            success = FALSE;
            break;
        }

        i++;
    }

    return !success;
}

int receive_data_frame(int fd, int sequence_number, char* buffer) {
    char frame[MAX_FRAME_SIZE];
    success = 1;
    int data_size = LOST_FRAME_ERROR;

    // define message construct
    char control = get_control(I, sequence_number);
    char inverse_control = get_control(I, !sequence_number);
    MessageConstruct construct = { .address = ADDRESS_SENDER_RECEIVER, .control = control, .inverse_control = inverse_control, .data = TRUE };
    enum set_state state = START;

    char buf[255] = { 0 };
    bool sequence_correct = TRUE;
    unsigned int i = 0;
    while (success) {
        // read from the port
        int res = read(fd, buf, 1);
        if (res <= 0) continue;

        apply_errors(buf);

        // printf("%x \n", buf[0]); // TEST

        // destuffing
        if (buf[0] == ESCAPE) {
            // reads the next character and saves it promptly without updating the state machine
            while ((res = read(fd, buf, 1)) <= 0);
            // printf("%x ", buf[0]); // TEST
            frame[i] = XOR(buf[0], STUFF_FLAG);

            i++;
            continue;
        }

        // update machine states
        if (!update_state(&state, buf[0], construct))
            sequence_correct = FALSE;

        // reset if finds flag for the first time
        if (state == FLAG_RCV)
            i = 0;
        else if (state == STOP) { // terminate if reads all
            data_size = i - 5; // i + 1 is the frame size i + 1 - 6, minus the non data flags
            data_size = parse_data(data_size, buffer, fd, i, frame, sequence_number);
            if (data_size < 0) {
                success = FALSE;
            }
            else {
                success = TRUE;
                frame[i] = buf[0];
            }

            break;
        }
        // updates the answer frame
        frame[i] = buf[0];
        i++;
    }

    bool equal = verify_repeated(sequence_correct, frame, i);

    if (equal)
        return REPEATED_FRAME_ERROR;

    return data_size;
}

int parse_data(int data_size, char* data_array, int fd, int i, char* frame, int sequence_number) {
    resize_array(frame, i, data_array, 4, data_size);
    char bcc2_result = xor_array(data_size, data_array);

    if (bcc2_result != frame[i - 1]) {
        char rej_frame[5];
        define_rej_frame(rej_frame, !sequence_number);

        send_unanswered_frame(fd, rej_frame, 5, "REJ");
        return LOST_FRAME_ERROR;
    }

    return data_size;
}

void define_ua_frame(char* ua_frame, int sender_to_receiver) {
    ua_frame[0] = FRAME_FLAG;
    if (sender_to_receiver)
        ua_frame[1] = ADDRESS_SENDER_RECEIVER;
    else
        ua_frame[1] = ADDRESS_RECEIVER_SENDER;
    ua_frame[2] = CONTROL_UA;
    ua_frame[3] = XOR(ua_frame[1], ua_frame[2]);
    ua_frame[4] = FRAME_FLAG;
}

/*******************************************************
 *                  PRIVATE FUNCTIONS                  *
********************************************************/

bool verify_repeated(bool sequence_correct, char* frame, int i) {
    bool equal = FALSE;
    // if the sequence number was wrong
    if (!sequence_correct) {
        equal = TRUE;

        // verify if frame is equal to the last one
        for (int j = 0; j < i && j < last_frame_size && equal; ++j)
            if (frame[j] != last_frame[j])
                equal = FALSE;
    }

    // save current frame for the next iteration
    for (int j = 0; j < i; ++j)
        last_frame[j] = frame[i];
    last_frame_size = i;

    return equal;
}

void apply_errors(char* buf) {
    usleep(TPROP);

    // incite random errors
    if (BER) {
        int percentage = rand() % BER;
        if (percentage == 1) {
            buf[0] = 0x00;
            if (OUTPUT) printf("Bit lost!\n");
        }
    }
}

void alarm_handler() {
    if (OUTPUT) printf("alarm # %d\n", ++count);
    success = 0;
}

void define_rej_frame(char* rej_frame, int sequence_number) {
    rej_frame[0] = FRAME_FLAG;
    rej_frame[1] = ADDRESS_RECEIVER_SENDER;
    rej_frame[2] = get_control(REJ, sequence_number);
    rej_frame[3] = XOR(rej_frame[1], rej_frame[2]);
    rej_frame[4] = FRAME_FLAG;
}

void subscribe_alarm_interruptions() {
    struct sigaction action;
    action.sa_handler = alarm_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGALRM, &action, NULL);
}
