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

    read_set_packet(fd);

    unsigned char ua_packet[5];
    define_ua_packet(ua_packet);

    send_ua_packet(ua_packet, fd);

    terminate_connection(fd, &oldtio);

    return 0;
}