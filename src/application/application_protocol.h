#include "util/util.h"

int send_start_control_packet(int file_size, char* file_name, enum unit_measure);

int send_end_control_packet();

int send_data_packet();