#include "error.h"

#include <stdio.h>

#include "../application/util/application.h"

void print_args_error(char* program_name) {
    fprintf(stderr, "Program called with errors.\n<%s> [%s] <filename>\n", program_name, VIRTUAL_PORTS_FLAG);
}

void print_config_port_error(char* port) {
    fprintf(stderr, "Error connecting to the %s port.\n", port);
}

void print_establish_connection_error() {
    fprintf(stderr, "Error handshaking the entities.\n");
}

void print_close_connection_error() {
    fprintf(stderr, "Error closing the connection between the entities.\n");
}

void print_lost_frame_error() {
    fprintf(stderr, "Frame lost.\n");
}

void print_lost_start_packet_error() {
    fprintf(stderr, "Start packet lost.\n");
}

void print_lost_end_packet_error() {
    fprintf(stderr, "End packet lost.\n");
}

void print_lost_data_packet_error() {
    fprintf(stderr, "Data packet lost.\n");
}

void print_invalid_ctrl_packet_type(char* type) {
    fprintf(stderr, "%d is an invalid control packet type.\n", type[0]);
}

void print_invalid_ctrl_packet() {
    fprintf(stderr, "Invalid control packet.\n");
}

void print_existing_file_error(char * filename){
    fprintf(stderr, "Already existing file %s. Stopping transmition.\n", filename);
}

void print_error(int error_code) {
    switch (error_code) {
    case ARGS_ERROR:
        print_args_error("progam_name");
        break;

    case CONFIG_PORT_ERROR:
        print_config_port_error("selected");
        break;

    case ESTABLISH_CONNECTION_ERROR:
        print_establish_connection_error();
        break;

    case CLOSE_CONNECTION_ERROR:
        print_close_connection_error();
        break;

    case LOST_FRAME_ERROR:
        print_lost_frame_error();
        break;

    case LOST_START_PACKET_ERROR:
        print_lost_start_packet_error();
        break;

    case LOST_END_PACKET_ERROR:
        print_lost_end_packet_error();
        break;

    case LOST_DATA_PACKET_ERROR:
        print_lost_data_packet_error();
        break;

    case INVALID_CTRL_PACKET_TYPE:
        print_invalid_ctrl_packet();
        break;

    default:
        break;
    }
}

void print_error_message(int error_code, char* arg) {
    switch (error_code) {
    case ARGS_ERROR:
        print_args_error(arg);
        break;

    case CONFIG_PORT_ERROR:
        print_config_port_error(arg);
        break;

    case INVALID_CTRL_PACKET_TYPE:
        print_invalid_ctrl_packet_type(arg);
        break;

    case EXISTING_FILE_ERROR:
        print_existing_file_error(arg);
        break;

    default:
        print_error(error_code);
        break;
    }
}