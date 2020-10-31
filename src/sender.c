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
    if ((virtual = check_sender_arguments(argc, argv, &filename)) < 0) {
        print_error_message(ARGS_ERROR, argv[0]);
        return ARGS_ERROR;
    }

    // verify size
    long size = get_file_size(filename);

    int error; // placeholder to store errors

    // send start control packet
    if ((error = send_start_control_packet(size, B, filename, virtual)) < 0)
        return error;
    printf("%s\n", CONNECTION_ESTABLISHED);

    // open file
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        print_error_message(FILE_ERROR, filename);
        return FILE_ERROR;
    }

    char data[DATA_SIZE];
    int read_size, total_read;
    int progress;
    float percentage;
    // while the file has more than DATA_SIZE bytes to read
    while ((read_size = fread(data, sizeof(char), DATA_SIZE, file)) > 0) {
        int length = send_data_packet(data, read_size);
        
        if (length < 0) {
            print_error(length);
            return length;
        }

        total_read += read_size;
        percentage = (float)total_read / (float)size * 100;
        progress = percentage / 5;

        if ((int)(percentage * 1000) % 2)
            printf("\e[?25l\r    \r%.1f%% [%.*s%.*s]", percentage, progress, "####################", 20 - progress, "                     ");
    }        

    printf("\e[?25l\r    \r%.1f%% [%.*s%.*s]\n", percentage, progress, "####################", 20 - progress, "                     ");

    //close file
    if ((error = fclose(file) < 0)) {
        perror(filename);
        print_error(FILE_ERROR);
        return FILE_ERROR;
    }

    printf("%s\n", FILE_SENT);

    // end packet
    if ((error = send_end_control_packet(size, B, filename)) < 0) {
        print_error(error);
        return error;
    }

    return 0;
}