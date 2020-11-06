/**
 * @file application_sender.h
 * @brief Application level sender handler.
 */

#include "../util/util.h"

/**
 * @brief Sends the start control packet.
 * 
 * @param file_size the file of the size that will be transferred.
 * @param unit      the unit measure of the size.
 * @param file_name the name of the file that will be transferred.
 * @param virtual   the bool that indicates if the ports are virtual or real.
 * @return int      the bytes written, negative in case of error.
 */
int send_start_control_packet(int file_size, enum unit_measure unit, char* file_name, bool virtual);

/**
 * @brief Sends a data packet.
 * 
 * @param buffer    the buffer to send.
 * @param data_size the size of the buffer.
 * @return long     the bytes written, negative in case of error.
 */
long send_data_packet(char *buffer, long data_size);

/**
 * @brief Sends the end control packet.
 * 
 * @param file_size the size of the file.
 * @param unit      the unit measure of the size.
 * @param file_name the name of the file.
 * @return int      0 in case of success, otherwise in case of error.
 */
int send_end_control_packet(int file_size, enum unit_measure unit, char* file_name);
