#include "application_protocol.h"

#include "application_packet.h"

// file_size in bytes
int send_start_control_packet(int file_size, char* file_name, enum unit_measure unit) {
    char* control_packet;

    char control = CONTROL_START;
    char t1 = TYPE_FILE_SIZE;

}
