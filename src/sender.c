#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link.h"
#include "sender/writenoncanonical.h"
#include "util/util.h"
#include "util/serial_port.h"

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], TRUE);
    if (fd > 0) printf("Connected\n");

    // IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    // define the information frame
    int data_size = 32;
    unsigned char data[32] = "ola flavia carvalho ~~carvalhido";
    unsigned char new_data[MAX_FRAME_SIZE];
    data_size = stuff_data(data, data_size, new_data);
    unsigned char message[data_size + 6];
    define_message_frame(message, new_data, data_size);

    // send the information frame
    int sent_message_error = send_information_frame(fd, message, data_size + 6);
    printf("Sent error: %d\n", sent_message_error);

    llclose(fd,1);

    unsigned char disc_frame[5];
    define_disc_frame(disc_frame);

    if ((sent_message_error = send_disc_frame(fd, disc_frame)))
        printf("Sent error: %d\n", sent_message_error);

    receive_disc_frame(fd);

    send_ua_frame();

    struct termios oldtio;
    if ((res = terminate_sender_connection(fd, &oldtio)))
        return res;
}