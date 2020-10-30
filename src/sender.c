#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "data_link/data_link.h"
#include "data_link/sender/writenoncanonical.h"
#include "data_link/util/serial_port.h"
#include "util/util.h"
#include "application/application_sender.h"
#include "application/util/application.h"
#include "error/error.h"

int main(int argc, char** argv) {
    // receive filename
    int virtual; // boolean or error
    char *filename = argv[0]; // placeholder
    if ((virtual = check_sender_arguments(argc, argv, &filename)) < 0)
        return ARGS_ERROR;

    printf("%s\n", filename);

    // verify size
    long size = get_file_size(filename);

    // send start control packet
    send_start_control_packet(size, B, filename, virtual);
    printf("%s\n", CONNECTION_ESTABLISHED);

    // open file
    FILE* file = fopen(filename, "r");
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
    send_end_control_packet(size, B, filename);
}