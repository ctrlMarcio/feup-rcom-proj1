#include "util.h"

#include <string.h>
#include <stdio.h>

#include "serial_port.h"
#include "../error/error.h"

int check_arguments(int argc, char** argv) {
    if ((argc < 2) || ((strcmp("/dev/ttyS10", argv[1]) != 0) && (strcmp("/dev/ttyS11", argv[1]) != 0))) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        return ARGS_ERROR;
    }

    return 0;
}

unsigned char xor_array(int data_size, unsigned char* data) {
    unsigned char result = data[0];
    for (unsigned int i = 1; i < data_size; i++) result = XOR(result, data[i]);
    return result;
}

void resize_array(unsigned char *array, unsigned size, unsigned char *res, unsigned first_index, unsigned res_size) {
    if (first_index < 0 || first_index >= size || (first_index + res_size) > size)
        res = array;

    int i = 0;
    for(; i < res_size; ++i)
        res[i] = array[first_index + i];
}

int stuff_data(unsigned char *data, int data_size, unsigned char *new_data) {
    int j = 0;
    for (int i = 0; i < data_size; ++i) {
        if (data[i] == FRAME_FLAG || data[i] == ESCAPE)
            new_data[j++] = ESCAPE;

        new_data[j++] = data[i];
    }

    return j;
}