#include "util.h"

#include <stdio.h>
#include <math.h>

long to_bytes(int amount, enum unit_measure unit) {
    long value = (long)amount;

    for (enum unit_measure tmp = unit; tmp > 0; tmp--)
        value *= 1024;

    return value;
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

long octets_to_size(char* octets, int octets_length) {
    long size = 0;
    
    for (int i = 0; i < octets_length; ++i) {
        size <<= 8;
        size |= octets[i];
    }

    return size;
}