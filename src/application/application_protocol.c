#include "application_protocol.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "application_packet.h"
#include "../util/util.h"

unsigned char amount_octets(long total_bytes);
void size_in_octets(long file_size, char* size_in_octets, unsigned arr_size);
int get_packet_size(int file_size, enum unit_measure unit, char* file_name);
void build_control_packet(char *control_packet, int file_size, enum unit_measure unit, char* file_name);

int send_start_control_packet(int file_size, enum unit_measure unit, char* file_name) {
    int packet_size = get_packet_size(file_size, unit, file_name);
    char control_packet[packet_size];
    build_control_packet(control_packet, file_size, unit, file_name);

    // TODO send the packet here

    return packet_size;
}

// PRIVATE FUNCTIONS

int get_packet_size(int file_size, enum unit_measure unit, char* file_name) {
    long bytes = to_bytes(file_size, unit);
    unsigned char l1 = amount_octets(bytes);

    int l2 = strlen(file_name);

    return 5 + l1 + l2;
}

void build_control_packet(char *control_packet, int file_size, enum unit_measure unit, char* file_name) {
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

unsigned char amount_octets(long total_bytes) {
    int bit_amount = ceil(log2(total_bytes));
    int byte_amount = bit_amount / 8;
    if (bit_amount % 8)
        byte_amount++;

    return byte_amount;
}

void size_in_octets(long file_size, char* size_in_octets, unsigned arr_size) {
    char octet = (unsigned char)file_size;

    for (int i = arr_size - 1; i >= 0; --i) {
        size_in_octets[i] = octet;
        file_size = file_size >> 8;
        octet = (unsigned char)file_size;
    }
}