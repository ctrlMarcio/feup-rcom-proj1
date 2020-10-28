#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "data_link/data_link.h"
#include "data_link/receiver/readnoncanonical.h"
#include "util/util.h"
#include "application/util/util.h"
#include "application/application_receiver.h"

int test_data_link(int argc, char** argv) {
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

    FILE* file = fopen(file_name, "w");

    // while bytes read < filesize
    // parse (probably in the protocol)
    // write to file

    // TEST
    char data[MAX_PACKET_SIZE];
    size = receive_data_packet(data);
    printf("%d\n\n", size);
    for (int i = 0; i < size; ++i)
        fputc(data[i], file);

    //close file
    fclose(file);

    // receive end packet
}