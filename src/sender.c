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
    char data[32] = "ola flavia carvalho ~~carvalhido";
    llwrite(fd, data, data_size);

    return llclose(fd);
}