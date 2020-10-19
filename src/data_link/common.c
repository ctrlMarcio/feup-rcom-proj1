#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "util/state_machine.h"

int count, success;

void define_rej_frame(unsigned char* rej_frame, int sequence_number);

int send_frame(int fd, unsigned char* frame, unsigned frame_size, char* type, bool retransmission, enum frame answer_type, bool sender_to_receiver, int sequence_number) {
    if (retransmission)
        return send_retransmission_frame(fd, frame, frame_size, type, answer_type, sender_to_receiver, sequence_number);
    else
        return send_unanswered_frame(fd, frame, frame_size, type);
}

int send_retransmission_frame(int fd, unsigned char* frame, unsigned frame_size, char* type, enum frame answer_type, bool sender_to_receiver, int sequence_number) {
    (void)signal(SIGALRM, alarm_handler);
    count = 0, success = 0;

    while (count < NR_ATTEMPTS) {
        if (success)
            break;

        int res = write(fd, frame, sizeof(unsigned char) * frame_size);
        alarm(TIMEOUT);
        printf("%d bytes sent in a %s frame\n", res, type);

        if (answer_type == UA)
            receive_frame(fd, 5, UA, sender_to_receiver, !sequence_number, FALSE);
        else if (answer_type == RR)
            receive_frame(fd, 5, RR, sender_to_receiver, !sequence_number, TRUE);
        else if (answer_type == DISC)
            receive_frame(fd, 5, DISC, !sender_to_receiver, !sequence_number, FALSE); // !sender_to_receiver bc DISC is not an answer, is a new request
    }

    return !success;
}

int send_unanswered_frame(int fd, unsigned char* frame, unsigned frame_size, char* type) {
    int res = write(fd, frame, sizeof(unsigned char) * frame_size);
    if (res < 0)
        return 1;

    printf("%d bytes sent in a %s frame\n", res, type);
    return 0;
}

int receive_frame(int fd, unsigned size, enum frame frame_type, bool sender_to_receiver, int sequence_number, bool expect_rej) {
    unsigned char frame[size];
    success = 1;

    // define message construct
    char address;
    if (sender_to_receiver)
        address = ADDRESS_SENDER_RECEIVER;
    else
        address = ADDRESS_RECEIVER_SENDER;
    char control = get_control(frame_type, sequence_number);
    bool data = (frame_type == I);
    MessageConstruct construct = { .address = address, .control = control, .data = data };
    enum set_state state = START;

    // define REJ message construct in case REJ frame is waited
    control = get_control(REJ, sequence_number);
    MessageConstruct rej = { .address = ADDRESS_RECEIVER_SENDER, .control = control, .data = FALSE };
    enum set_state rej_state = START;

    char buf[255] = { 0 };

    unsigned int i = 0;
    while (success) {
        // read from the port
        int res = read(fd, buf, 1);
        if (res < 0) continue;

        // destuffing
        if (data && buf[0] == ESCAPE) {
            // reads the next character and saves it promptly without updating the state machine
            read(fd, buf, 1);
            frame[i] = buf[0];

            i++;
            continue;
        }

        // update machine states
        update_state(&state, buf[0], construct);

        if (expect_rej)
            update_state(&rej_state, buf[0], rej);

        // reset if finds flag for the first time
        if (state == FLAG_RCV && (!expect_rej || rej_state == FLAG_RCV))
            i = 0;
        else if (state == STOP) { // terminate if reads all
            // TODO this is ugly, save the data array and test in smaller functions :)
            if (data) {
                int data_size = i - 5; // i + 1 is the frame size i + 1 - 6, minus the non data flags
                unsigned char data_array[data_size];
                resize_array(frame, i, data_array, 4, data_size);
                unsigned char bcc2_result = xor_array(data_size, data_array);
                if (bcc2_result != frame[i - 1]) {
                    unsigned char rej_frame[5];
                    define_rej_frame(rej_frame, !sequence_number);

                    send_unanswered_frame(fd, rej_frame, 5, "REJ");
                    return receive_frame(fd, MAX_FRAME_SIZE, I, TRUE, sequence_number, TRUE); // TODO temporary, let llread or something handle this or just continue mby try it
                }
            }
            success = TRUE;
            frame[i] = buf[0];
            break;
        }
        else if (rej_state == STOP) { // if it reads a REJ frame, returns unsuccess
            alarm(0);                   // anticipates alarm
            alarm_handler();
            break;
        }

        // updates the answer frame
        frame[i] = buf[0];
        // printf("%c", frame[i]); // TEST

        i++;
    }

    return success;
}

void define_ua_frame(unsigned char* ua_frame, int sender_to_receiver) {
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

void alarm_handler() {
    printf("alarm # %d\n", ++count);
    success = 0;
}

void define_rej_frame(unsigned char* rej_frame, int sequence_number) {
    rej_frame[0] = FRAME_FLAG;
    rej_frame[1] = ADDRESS_RECEIVER_SENDER;
    rej_frame[2] = get_control(REJ, sequence_number);
    rej_frame[3] = XOR(rej_frame[1], rej_frame[2]);
    rej_frame[4] = FRAME_FLAG;
}