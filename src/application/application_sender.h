#include "util/util.h"
#include "../util/util.h"

int send_start_control_packet(int file_size, enum unit_measure, char* file_name, bool virtual);

long send_data_packet(char *buffer, long data_size);

int send_end_control_packet(int file_size, enum unit_measure unit, char* file_name);
