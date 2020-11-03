#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "data_link/data_link.h"
#include "data_link/receiver/readnoncanonical.h"
#include "util/util.h"
#include "application/application_receiver.h"
#include "application/util/util.h"
#include "application/util/application.h"
#include "error/error.h"

int main(int argc, char** argv) {
    srand(time(0)); // TEST

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

    // open file
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror(filename);
        print_error_message(FILE_ERROR, filename);
        return FILE_ERROR;
    }

    clock_t start = clock();

    // initializes the total bytes read
    long total_read = 0;

    char data[MAX_PACKET_SIZE];
    long bytes_read;
    while (total_read < size) {
        bytes_read = receive_data_packet(data);
        fwrite(data, sizeof(char), bytes_read, file);

        total_read += bytes_read;

        print_progess(total_read, size, start);
    }

    print_progess(size, size, start);
    printf("\n");

    // close file
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