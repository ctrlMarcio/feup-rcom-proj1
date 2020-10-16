/*Non-Canonical Input Processing*/
#include "writenoncanonical.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../error/error.h"
#include "../util/serial_port.h"
#include "../util/state_machine.h"
#include "../util/util.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int count = 0, success = 0;

void alarm_handler();
void define_set_frame(unsigned char *set_frame);
int read_answer(int fd);
int connect_to_receiver(int fd, unsigned char *set_frame, int attempts, int timeout);

int attempt_establishment(int fd) {
    unsigned char set_frame[5];
    define_set_frame(set_frame);

    return connect_to_receiver(fd, set_frame, NR_ATTEMPTS, TIMEOUT);
}

int define_message_frame(unsigned char *message, int control_setter, unsigned char *data) {
    // FIXME
    message[0] = FRAME_FLAG;
    message[1] = ADDRESS_SENDER_RECEIVER;
    if (control_setter)
        message[2] = CONTROL_I_ONE;
    else
        message[2] = CONTROL_I_ZERO;
    message[3] = XOR(message[1], message[2]);
    
    for (unsigned int i = 0; i < 249; i++) // FIXME
        message[i+4] = data[i];

    message[253] = xor_array(sizeof(data) / sizeof(unsigned char), data); // FIXME
    message[254] = FRAME_FLAG;

    return 0;
}

int send_message(int fd, unsigned char *message) {
    write(fd, message, sizeof(unsigned char) * 255); // FIXME
    return 0;
}

int terminate_sender_connection(int fd, struct termios *oldtio) {
    sleep(1);
    if (tcsetattr(fd, TCSANOW, oldtio) == -1) {
        perror("tcsetattr");
        return CONFIG_PORT_ERROR;
    }

    close(fd);
    return 0;
}

void define_set_frame(unsigned char *set_frame) {
    set_frame[0] = FRAME_FLAG;
    set_frame[1] = ADDRESS_SENDER_RECEIVER;
    set_frame[2] = CONTROL_SET;
    set_frame[3] = XOR(ADDRESS_SENDER_RECEIVER, CONTROL_SET);
    set_frame[4] = FRAME_FLAG;
}

int read_answer(int fd) {
    unsigned char answer_frame[5];
    success = 1;
    int i = 0;

    MessageConstruct ua = {.address = ADDRESS_SENDER_RECEIVER, .control = CONTROL_UA, .data = FALSE};
    enum set_state state = START;

    char buf[255] = {0};

    while (success) {
        int res = read(fd, buf, 1);
        if (res < 0)
            continue;
        answer_frame[i] = buf[0];
        update_state(&state, answer_frame[i], ua);

        if (state == STOP) {
            success = 1;
            break;
        }
        i++;
    }

    return success;
}

void alarm_handler() { 
    printf("alarm # %d\n", count);
    success = 0;
    count++;
}

int connect_to_receiver(int fd, unsigned char *set_frame, int attempts, int timeout) {
    (void)signal(SIGALRM, alarm_handler);

    // receives the answer_frame
    while (count < timeout) {
        if (success)
            break;

        // sends the set
        int res = write(fd, set_frame, sizeof(unsigned char) * 5);
        alarm(timeout);
        printf("%d bytes sent\n", res);

        read_answer(fd);
    }

    return !success;
}