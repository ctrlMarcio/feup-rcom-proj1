#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link/data_link.h"
#include "data_link/sender/writenoncanonical.h"
#include "data_link/util/serial_port.h"
#include "util/util.h"
#include "application/application_sender.h"
#include "application/util/protocol.h"
#include "application/util/io.h"
#include "error/error.h"

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
    if (argc != 2)
        return ARGS_ERROR;
    char* file_name = argv[1];

    // verify size
    long size = get_file_size(file_name);

    // send start control packet
    send_start_control_packet(size, B, file_name);
    printf("%s\n", CONNECTION_ESTABLISHED);

    // open file
    FILE* file = fopen(file_name, "r");
    // TODO verify open error

    char data[DATA_SIZE];
    int read_size;
    // while the file has more than DATA_SIZE bytes to read
    while ((read_size = fread(data, sizeof(char), DATA_SIZE, file)) == DATA_SIZE) {
        send_data_packet(data, read_size); // TODO fail if the sending fails
    }

    // if there are still some information in the file
    if (read_size > 0)
        send_data_packet(data, read_size);

    //close file
    fclose(file);

    printf("%s\n", FILE_SENT);

    // end packet
}