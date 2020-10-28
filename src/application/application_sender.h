#include "util/util.h"

int send_start_control_packet(int file_size, enum unit_measure, char* file_name);

int send_end_control_packet();

long send_data_packet(char *buffer, long data_size);