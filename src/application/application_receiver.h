#include "../util/util.h"

long receive_start_control_packet(char* file_name, bool virtual);

long receive_data_packet(char* data); 

int receive_end_control_packet(char * file_name, long total_read);
