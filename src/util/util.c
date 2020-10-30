#include "util.h"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../data_link/util/serial_port.h"
#include "../error/error.h"
#include "../application/util/application.h"

int check_sender_arguments(int argc, char** argv, char** filename) {
    if (argc != 2 && argc != 3)
        return ARGS_ERROR;

    bool virtual = FALSE;
    if (argc == 3) {
        if (!strcmp(argv[1], VIRTUAL_PORTS_FLAG) || !strcmp(argv[1], VIRTUAL_PORTS_EXTENDED_FLAG))
            *filename = argv[2];
        else if (!strcmp(argv[2], VIRTUAL_PORTS_FLAG) || !strcmp(argv[2], VIRTUAL_PORTS_EXTENDED_FLAG))
            *filename = argv[1];
        else
            return ARGS_ERROR;
        virtual = TRUE;
    }
    else {
        *filename = argv[1];
    }

    return virtual;
}

int check_receiver_arguments(int argc, char** argv) {
    if (argc > 2)
        return ARGS_ERROR;
    bool virtual = FALSE;
    if (argc == 2) {
        if (!strcmp(argv[1], VIRTUAL_PORTS_FLAG) || !strcmp(argv[1], VIRTUAL_PORTS_EXTENDED_FLAG))
            virtual = TRUE;
        else
            return ARGS_ERROR;
    }

    return virtual;
}

unsigned char xor_array(int data_size, char* data) {
    unsigned char result = data[0];
    for (unsigned int i = 1; i < data_size; i++) result = XOR(result, data[i]);
    return result;
}

void resize_array(char* array, unsigned size, char* res, unsigned first_index, unsigned res_size) {
    if (first_index < 0 || first_index >= size || (first_index + res_size) > size)
        res = array;

    int i = 0;
    for (; i < res_size; ++i)
        res[i] = array[first_index + i];
}

int stuff_data(char* data, int data_size, char* new_data) {
    int j = 0;
    for (int i = 0; i < data_size; ++i) {
        if (data[i] == FRAME_FLAG || data[i] == ESCAPE) {
            new_data[j++] = ESCAPE;
            new_data[j++] = XOR(data[i], STUFF_FLAG);
        }
        else {
            new_data[j++] = data[i];
        }
    }

    return j;
}

int append_array(char* original, int size, char* to_append, int to_append_size) {
    int i = 0;
    for (; i < to_append_size; ++i)
        original[size + i] = to_append[i];

    return size + i;
}

long get_file_size(char* file_name) {
    struct stat st;
    long size;
    stat(file_name, &st);
    size = st.st_size;

    return size;
}