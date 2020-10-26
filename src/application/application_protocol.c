#include "application_protocol.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "application_packet.h"
#include "util/protocol.h"
#include "util/util.h"
#include "../util/util.h"
#include "../data_link/data_link.h"
#include "../data_link/util/serial_port.h"
#include "../error/error.h"

long file_size;

int get_packet_size(int file_size, enum unit_measure unit, char* file_name);
void build_control_packet(char* control_packet, int file_size, enum unit_measure unit, char* file_name);
long parse_start_control_packet(char* file_name, char* packet, int packet_size);

int send_start_control_packet(int file_size, enum unit_measure unit, char* file_name) {
    int packet_size = get_packet_size(file_size, unit, file_name);
    char control_packet[packet_size];
    build_control_packet(control_packet, file_size, unit, file_name);

    int fd = llopen("/dev/ttyS10", 1);      // TODO: save FD in global variable for later use and replace the literal string
    if (fd < 0)
        return CONFIG_PORT_ERROR;

    int written_bytes = llwrite(fd, control_packet, packet_size);
    if (written_bytes < 0)
        return LOST_START_PACKET_ERROR;

    return written_bytes;
}

long receive_start_control_packet(char* file_name) {
    int fd = llopen("/dev/ttyS11", 0);      // TODO: replace the literal string
    if (fd < 0)
        return CONFIG_PORT_ERROR;

    char buffer[MAX_FRAME_SIZE];
    int read_bytes = llread(fd, buffer);
    if (read_bytes < 0)
        return LOST_START_PACKET_ERROR;

    return parse_start_control_packet(file_name, buffer, read_bytes);
}

// PRIVATE FUNCTIONS

int get_packet_size(int file_size, enum unit_measure unit, char* file_name) {
    long bytes = to_bytes(file_size, unit);
    unsigned char l1 = amount_octets(bytes);

    int l2 = strlen(file_name);

    return 5 + l1 + l2;
}

void build_control_packet(char* control_packet, int file_size, enum unit_measure unit, char* file_name) {
    char control = CONTROL_START;
    char t1 = TYPE_FILE_SIZE;

    long bytes = to_bytes(file_size, unit);
    unsigned char l1 = amount_octets(bytes);
    char v1[l1];
    size_in_octets(bytes, v1, l1);

    char t2 = TYPE_FILE_NAME;
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

long parse_start_control_packet(char* file_name, char* packet, int packet_size) {
    int l1, l2;
    char* v1;
    long size = 0;

    for (int i = 1; i < packet_size; ++i) {
        int type = packet[i++];

        switch (type)
        {
        case TYPE_FILE_SIZE:
            l1 = packet[i];
            v1 = (char*)malloc(sizeof(char) * l1);

            for (int j = 0; j < l1; j++)
                v1[j] = packet[++i];

            size = octets_to_size(v1, l1);

            break;

        case TYPE_FILE_NAME:
            l2 = packet[i];

            for (int j = 0; j < l2; j++)
                file_name[j] = packet[++i];

            break;
            
        default:
            return INVALID_CTRL_PACKET_TYPE;
        }
    }

    free(v1);

    return size;
}