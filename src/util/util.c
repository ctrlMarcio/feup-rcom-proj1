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

bool replace_file(char *filename) {
    //ask the user to replace file or skip
    printf("The following file %s already exists in the destination folder. Do you wish to replace the existing file? (Y/N) ", filename);

    //gets input
    char answer[MAX_USER_INPUT_SIZE];
    fgets(answer, MAX_USER_INPUT_SIZE, stdin);

    answer[strlen(answer) - 1] = '\0';

    if ( strcmp(answer,"Y") == 0 || strcmp(answer, "YES") == 0 || strcmp(answer,"y") == 0 || strcmp(answer,"yes") == 0 || strcmp(answer, "s") == 0 || strcmp(answer, "sim") == 0 || strcmp(answer, "S") == 0 || strcmp(answer, "SIM") == 0 || strcmp(answer ,"sim senhora minha cara colega sim sim sim") == 0)
        return TRUE;
    
    if ( strcmp(answer, "N") == 0 || strcmp(answer, "NO") == 0 || strcmp(answer, "n") == 0 || strcmp(answer, "no") == 0 || strcmp(answer, "não") == 0 || strcmp(answer, "NÃO") == 0 || strcmp(answer, "não sua cara de cu, trabalha não é para isto que te pago") == 0) //TODO: fix this or meme this idc
        return FALSE;
    
    return TRUE;
}

void print_progess(int total_read, int size, clock_t start) {
    float percentage = (float)total_read / (float)size * 100;
    int progress = percentage / 5;

    if ((int)(percentage * 1000) % 2 || percentage == 100) {
        clock_t end = clock();
        float elapsed = (float)(end - start) / CLOCKS_PER_SEC;

        // TODO use struct "size"
        char* type = "B";
        float read = (float)total_read;

        if (read > 1024) {
            type = "KB";
            read /= 1024;
        }
        if (read > 1024) {
            type = "MB";
            read /= 1024;
        }
        if (read > 1024) {
            type = "GB";
            read /= 1024;
        }

        // TODO use function/struct
        char* time = "s";
        float remaining = 100 * elapsed / percentage - elapsed;

        if (remaining > 60) {
            time = "min";
            remaining /= 60;
        }
        if (remaining > 60) {
            time = "h";
            remaining /= 60;
        }

        // TODO use function/struct
        char* speed_type = "B/s";
        float speed = (float) total_read / elapsed;

        if (speed > 1024) {
            speed_type = "KB/s";
            speed /= 1024;
        }
        if (speed > 1024) {
            speed_type = "MB/s";
            speed /= 1024;
        }
        if (speed > 1024) {
            speed_type = "GB/s";
            speed /= 1024;
        }

        printf("\e[?25l\r%.1f%% [%.*s%.*s]    %0.2fs elapsed    %0.2f %s    %0.2f %s    %0.2f%s remaining%.*s", percentage, progress, "####################", 20 - progress, "                     ", elapsed, read, type, speed, speed_type, remaining, time, 10, "                     ");
    }
}