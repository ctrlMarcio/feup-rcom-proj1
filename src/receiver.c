#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "data_link/data_link.h"
#include "data_link/receiver/readnoncanonical.h"
#include "util/util.h"
#include "application/application_receiver.h"
#include "application/util/util.h"
#include "application/util/application.h"
#include "error/error.h"

int main(int argc, char** argv) {
    int virtual; // boolean or negative (error)
    if ((virtual = check_receiver_arguments(argc, argv)) < 0) {
        print_error_message(ARGS_ERROR, argv[0]);
        return ARGS_ERROR;
    }

    char filename[MAX_FILE_NAME_SIZE];
    long size;
    if ((size = receive_start_control_packet(filename, virtual)) < 0)
        return size;


    printf("%s\n", CONNECTION_ESTABLISHED);


    // verifies if the user wants to replace the file or skip
    bool replace = TRUE;
    if (access(filename, F_OK) != -1) {
        replace = replace_file(filename);
    }

    if (!replace) {
        print_error_message(EXISTING_FILE_ERROR, filename);
        // TODO: send frame to other side so sender finishes

        return EXISTING_FILE_ERROR;
    }

    //open file
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror(filename);
        print_error_message(FILE_ERROR, filename);
        return FILE_ERROR;
    }

    // initializes the total bytes read
    long total_read = 0;

    char data[MAX_PACKET_SIZE];
    int progress = 0;
    float percentage;
    long bytes_read;
    while (total_read < size) {
        bytes_read = receive_data_packet(data);
        fwrite(data, sizeof(char), bytes_read, file);

        total_read += bytes_read;

        percentage = (float)total_read / (float)size * 100;
        progress = percentage / 5;

        if ((int) (percentage * 1000) % 2)
            printf("\e[?25l\r    \r%.1f%% [%.*s%.*s]", percentage, progress, "####################", 20 - progress, "                     ");
    }
    printf("\e[?25l\r    \r%.1f%% [%.*s%.*s]\n", percentage, progress, "####################", 20 - progress, "                     ");

    //close file
    int error;
    if ((error = fclose(file) < 0)) {
        perror(filename);
        print_error(FILE_ERROR);
        return FILE_ERROR;
    }

    printf("%s: %s\n", FILE_RECEIVED, filename);

    // receive end packet
    if ((error = receive_end_control_packet(filename, total_read)) < 0) {
        print_error(error);
        return error;
    }

    return 0;
}