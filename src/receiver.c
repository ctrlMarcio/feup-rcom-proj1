#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "util/util.h"
#include "receiver/readnoncanonical.h"

int main(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    struct termios oldtio;
    int fd = open_reading_serial_port(argv[1], &oldtio);

    receive_set_frame(fd);

    unsigned char ua_frame[5];
    define_ua_frame(ua_frame);

    send_ua_frame(ua_frame, fd);

    receive_frame(fd, 0);

    terminate_connection(fd, &oldtio);

    return 0;
}