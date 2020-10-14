#include <string.h>
#include <stdio.h>

#include "util.h"
#include "../error/error.h"

int check_arguments(int argc, char **argv) {
    if ((argc < 2) || ((strcmp("/dev/ttyS10", argv[1]) != 0) && (strcmp("/dev/ttyS11", argv[1]) != 0))) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        return ARGS_ERROR;
    }

    return 0;
}

unsigned char xor_array(int data_size, unsigned char * data){
    unsigned char result=data[0];
    for(unsigned int i=1; i<data_size; i++) result=XOR(result,data[i]);
    return result;
}