#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "data_link/data_link.h"
#include "data_link/receiver/readnoncanonical.h"
#include "util/util.h"
#include "application/util/util.h"
#include "application/application_protocol.h"

int test_data_link(int argc, char **argv) {
    int res;
    if ((res = check_arguments(argc, argv)))
        return res;

    int fd = llopen(argv[1], FALSE);

    char asdlfkmsdalkfmasdf[MAX_FRAME_SIZE];
    llread(fd, asdlfkmsdalkfmasdf);
    
    char sakdfjnaskdjasd[MAX_FRAME_SIZE];
    llread(fd, sakdfjnaskdjasd);

    return llclose(fd);
}

int main(int argc, char** argv) {
    char file_name[1024];
    long size = receive_start_control_packet(file_name);

    printf("%ld %s\n", size, file_name);

    // while bytes read < filesize
    // parse (probably in the protocol)
    // write to file

    // receive end packet
}