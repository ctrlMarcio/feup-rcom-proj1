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

    // FIXME IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    // // define the information frame
    // unsigned char message[255]; // FIXME
    // unsigned char data[33] = "ola flavia carvalho ~~carvalhido";
    // define_message_frame(message, 0, data);

    // // send the information frame
    // int sent_message_error = send_message(fd, message);
    // printf("Sent error: %d\n", sent_message_error);

    // if ((res = terminate_connection(fd, &oldtio)))
    //     return res;
}