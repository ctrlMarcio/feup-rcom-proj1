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
    // TODO message errors
    int virtual; // boolean or negative (error)
    if ((virtual = check_receiver_arguments(argc, argv)) < 0)
        return ARGS_ERROR;

    char file_name[1024];
    long size = receive_start_control_packet(file_name, virtual);
    printf("%s\n", CONNECTION_ESTABLISHED);

    FILE* file = fopen(file_name, "w");

    // TODO do something if file exists

    // initializes the total bytes read
    long total_read = 0;

    char data[MAX_PACKET_SIZE];
    printf("0%%");
    while (total_read < size) {
        long bytes_read = receive_data_packet(data);
        fwrite(data, sizeof(char), bytes_read, file);

        total_read += bytes_read;
        printf("\r   \r%.2f%%", (float) total_read / (float) size * 100);
    }
    printf("\n");
    
    //close file
    fclose(file);
    printf("%s: %s\n", FILE_RECEIVED, file_name);

    // receive end packet
    receive_end_control_packet(file_name, total_read);  // TODO: tratar dos erros
}