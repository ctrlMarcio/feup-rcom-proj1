#include "data_link.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "../util/util.h"
#include "../error/error.h"
#include "receiver/readnoncanonical.h"
#include "sender/writenoncanonical.h"
#include "util/serial_port.h"
#include "util/state_machine.h"

#define BAUDRATE B38400

struct termios oldtio;
char write_sequence_number = 0;
enum entity entity;

int open_serial_port(char* port);
void define_disc_frame(char* disc_frame, bool sender_to_receiver);
int close_sender(int fd);
int close_receiver(int fd);
void receive_disc_frame(int fd, bool sender);

int llopen(char* port, enum entity open_entity) {
    int fd = open_serial_port(port);
    if (fd < 0)
        return CONFIG_PORT_ERROR;

    entity = open_entity;

    if (open_entity == SENDER) {
        if (attempt_establishment(fd))
            return ESTABLISH_CONNECTION_ERROR;
    }
    else {
        answer_establishment(fd);
    }
    return fd;
}

int llwrite(int fd, char* buffer, int length) {
    char new_data[MAX_FRAME_SIZE];
    char bcc2 = xor_array(length, buffer);

    char bufferBcc[length + 1];
    for (int i = 0; i < length; ++i)
        bufferBcc[i] = buffer[i];
    bufferBcc[length] = bcc2;

    length = stuff_data(bufferBcc, length + 1, new_data);
    char message[length + 5];
    define_message_frame(message, new_data, length, write_sequence_number);

    // send the information frame
    int sent_error = send_information_frame(fd, message, length + 5, write_sequence_number);
    if (sent_error)
        return LOST_FRAME_ERROR;

    write_sequence_number = 1 - write_sequence_number;
    return length;
}

int llread(int fd, char* buffer) {
    int size;
    if ((size = answer_information(fd, buffer)) < 0)
        return LOST_FRAME_ERROR;

    return size;
}

int llclose(int fd) {
    if (entity == SENDER)
        return close_sender(fd);
    else
        return close_receiver(fd);
}

/* ******************************************************************* */
/* ************************ PRIVATE FUNCTIONS ************************ */
/* ******************************************************************* */

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

    if (OUTPUT) printf("New termios structure set\n");
    return fd;
}

int close_sender(int fd) {
    char disc_frame[5];
    define_disc_frame(disc_frame, TRUE);

    if (send_retransmission_frame(fd, disc_frame, 5, "DISC", DISC, TRUE))
        return CLOSE_CONNECTION_ERROR;

    char ua_frame[5];
    define_ua_frame(ua_frame, FALSE);

    if (send_unanswered_frame(fd, ua_frame, 5, "UA"))
        return CLOSE_CONNECTION_ERROR;

    return terminate_sender_connection(fd, &oldtio);
}

int close_receiver(int fd) {
    if (receive_frame(fd, DISC, TRUE, FALSE))
        return CLOSE_CONNECTION_ERROR;

    char disc_frame[5];
    define_disc_frame(disc_frame, FALSE);

    if (send_retransmission_frame(fd, disc_frame, 5, "DISC", UA, FALSE))
        return CLOSE_CONNECTION_ERROR;

    return terminate_sender_connection(fd, &oldtio);
}

void define_disc_frame(char* disc_frame, bool sender_to_receiver) {
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
    char disc_frame[5];
    char buf[1];

    char address;
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

/* 🥰🧡💛💚💙💜🤎🖤🤍💕💞💓💗💖💘💝 */