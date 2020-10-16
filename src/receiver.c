#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "data_link.h"
#include "util/util.h"
#include "receiver/readnoncanonical.h"

int main(int argc, char** argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], FALSE);

    // IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    answer_information(fd);

    struct termios oldtio; // TODO
    terminate_receiver_connection(fd, &oldtio);

    return 0;
}