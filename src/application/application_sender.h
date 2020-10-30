#include "util/util.h"
#include "../util/util.h"

int send_start_control_packet(int file_size, enum unit_measure, char* file_name, bool virtual);

int send_end_control_packet();

long send_data_packet(char *buffer, long data_size);