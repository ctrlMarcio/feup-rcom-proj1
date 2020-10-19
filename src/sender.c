#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link/data_link.h"
#include "data_link/sender/writenoncanonical.h"
#include "data_link/util/serial_port.h"
#include "util/util.h"

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], TRUE);

    // IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    // define the information frame
    int data_size = 32;
    unsigned char data[32] = "ola flavia carvalho ~~carvalhido";
    unsigned char new_data[MAX_FRAME_SIZE];
    data_size = stuff_data(data, data_size, new_data);
    unsigned char message[data_size + 6];
    define_message_frame(message, new_data, data_size);

    // send the information frame
    send_retransmission_frame(fd, message, data_size + 6, "I", RR, TRUE, 0);

    return llclose(fd,TRUE);
}