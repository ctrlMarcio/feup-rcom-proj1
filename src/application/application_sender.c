#include "application_sender.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/application.h"
#include "../util/util.h"
#include "../data_link/data_link.h"
#include "../data_link/util/serial_port.h"
#include "../error/error.h"

int sequence_number = 0;
int fd;

int get_packet_size(int file_size, enum unit_measure unit, char* file_name);
void build_control_packet(char* control_packet, int file_size, enum unit_measure unit, char* file_name, bool start);
int build_data_packet(char* data, long data_size, char* buffer);

int send_start_control_packet(int file_size, enum unit_measure unit, char* file_name, bool virtual) {
    int packet_size = get_packet_size(file_size, unit, file_name);
    char control_packet[packet_size];
    build_control_packet(control_packet, file_size, unit, file_name, TRUE);

    char *port;

    if (virtual)
        port = VIRTUAL_SENDER_PORT;
    else
        port = DEFAULT_PORT;
    
    fd = llopen(port, SENDER);

    if (fd < 0) {
        print_error_message(CONFIG_PORT_ERROR, port);
        return CONFIG_PORT_ERROR;
    }

    int written_bytes = llwrite(fd, control_packet, packet_size);
    if (written_bytes < 0) {
        print_error(LOST_START_PACKET_ERROR);
        return LOST_START_PACKET_ERROR;
    }

    return written_bytes;
}

long send_data_packet(char* data, long data_size) {
    char buffer[MAX_PACKET_SIZE];
    int buffer_size = build_data_packet(data, data_size, buffer);

    int sent;
    if ((sent = llwrite(fd, buffer, buffer_size)) < 0)
        return sent;

    sequence_number = (sequence_number + 1) % 255;

    return sent;
}

int send_end_control_packet(int file_size, enum unit_measure unit, char* file_name) {
    int packet_size = get_packet_size(file_size, unit, file_name);
    char control_packet[packet_size];
    build_control_packet(control_packet, file_size, unit, file_name, FALSE);

    int written_bytes = llwrite(fd, control_packet, packet_size);
    if (written_bytes < 0)
        return LOST_START_PACKET_ERROR;

    return llclose(fd);

}


// PRIVATE FUNCTIONS

int get_packet_size(int file_size, enum unit_measure unit, char* file_name) {
    long bytes = to_bytes(file_size, unit);
    unsigned char l1 = amount_octets(bytes);

    int l2 = strlen(file_name);

    return 5 + l1 + l2;
}

void build_control_packet(char* control_packet, int file_size, enum unit_measure unit, char* file_name, bool start) {
    char control;
    if (start)
        control = CONTROL_START;
    else
        control = CONTROL_END;

    char t1 = FILE_SIZE_TYPE;

    long bytes = to_bytes(file_size, unit);
    unsigned char l1 = amount_octets(bytes);
    char v1[l1];
    size_in_octets(bytes, v1, l1);

    char t2 = FILE_NAME_TYPE;
    int l2 = strlen(file_name);
    int i = 0;

    control_packet[i++] = control;
    control_packet[i++] = t1;
    control_packet[i++] = l1;
    i = append_array(control_packet, i, v1, l1);
    control_packet[i++] = t2;
    control_packet[i++] = l2;
    append_array(control_packet, i, file_name, l2);
}

int build_data_packet(char* data, long data_size, char* buffer) {

    buffer[0] = CONTROL_DATA;
    buffer[1] = sequence_number;

    char l2l1[2];
    size_in_octets(data_size, l2l1, 2);
    buffer[2] = l2l1[0];
    buffer[3] = l2l1[1];

    return append_array(buffer, 4, data, data_size);
}