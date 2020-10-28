#include "application_receiver.h"

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

int fd;

long parse_start_control_packet(char* file_name, char* packet, int packet_size);
long parse_data_packet(char* packet, int packet_size, char* data);

long receive_start_control_packet(char* file_name) {
    fd = llopen("/dev/ttyS11", 0);      // TODO: replace the literal string
    if (fd < 0)
        return CONFIG_PORT_ERROR;

    char buffer[MAX_FRAME_SIZE];
    int read_bytes = llread(fd, buffer);
    if (read_bytes < 0)
        return LOST_START_PACKET_ERROR;

    return parse_start_control_packet(file_name, buffer, read_bytes);
}

// TODO returns the data (information mesmo) size e a data no parametro
long receive_data_packet(char* data) {
    char packet[MAX_PACKET_SIZE];
    int packet_size = llread(fd, packet);
    if (packet_size < 0)
        return LOST_START_PACKET_ERROR;

    return parse_data_packet(packet, packet_size, data);
}

// PRIVATE FUNCTIONS

long parse_start_control_packet(char* file_name, char* packet, int packet_size) {
    if (packet[0] != CONTROL_START)
        return LOST_START_PACKET_ERROR;

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

long parse_data_packet(char* packet, int packet_size, char* data) {
    if (packet[0] != CONTROL_DATA)
        return LOST_DATA_PACKET;

    char sequence_number = packet[1]; // FIXME what is this for?
    char l2 = packet[2]; // FIXME what is this for too?
    char l1 = packet[3];

    char l2l1[2] = { l2, l1 };

    long size = octets_to_size(l2l1, 2);

    int i;
    for (i = 0; i < packet_size - 4; ++i)
        data[i] = packet[i + 4];

    if (size != i)
         return LOST_DATA_PACKET;

    return i;
}