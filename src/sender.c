#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link.h"
#include "sender/writenoncanonical.h"
#include "util/util.h"

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], TRUE);
    if (fd > 0) printf("Connected\n");

    // IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    // define the information frame
    int data_size = 32;
    unsigned char data[32] = "ola flavia carvalho aacarvalhido";
    // TODO stuff data and update data size
    unsigned char message[data_size + 6];
    define_message_frame(message, data, data_size);

    // send the information frame
    int sent_message_error = send_information_frame(fd, message, data_size + 6);
    printf("Sent error: %d\n", sent_message_error);

    struct termios oldtio;
    if ((res = terminate_sender_connection(fd, &oldtio)))
        return res;
}