#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link/data_link.h"
#include "data_link/sender/writenoncanonical.h"
#include "data_link/util/serial_port.h"
#include "util/util.h"
#include "application/application_protocol.h"

int test_data_link(int argc, char** argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], TRUE);

    // define the information frame
    int data_size = 33;
    char data[33] = "ola marcio {carvalho} ~carvalhido";
    llwrite(fd, data, data_size);

    data_size = 17;
    char bro[17] = "macaquinho ihihih";
    llwrite(fd, bro, data_size);

    return llclose(fd);
}

int main(int argc, char** argv) {
    // receive filename
    // verify size
    send_start_control_packet(502, GB, "marcio");

    // while
    // read X bytes
    // send those bytes

    // end packet
}