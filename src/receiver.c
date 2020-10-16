#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "data_link.h"
#include "util/util.h"
#include "receiver/readnoncanonical.h"

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    llopen(argv[1], FALSE);

    // FIXME IMPLEMENTED UNTIL HERE WITH ASSIGNMENT CALLS !!!!!!!

    // receive_frame(fd, 0);

    // terminate_connection(fd, &oldtio);

    return 0;
}