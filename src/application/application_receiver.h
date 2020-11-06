/**
 * @file application_receiver.h
 * @brief Application level receiver handler.
 */

#include "../util/util.h"

 /**
  * @brief Receives the start control packet.
  *
  * @param file_name the name of the file to transfer, will be filled in this function.
  * @param virtual   the bool that verifies if the ports used are virtual or not.
  * @return long     the size of the file to transfer.
  */
long receive_start_control_packet(char* file_name, bool virtual);

/**
 * @brief Receives a data packet.
 *
 * @param data      the data array to filled with the received information.
 * @return long     the size of the data.
 */
long receive_data_packet(char* data);

/**
 * @brief Receives the end and last control packet.
 * Verifies if there was some kind of error with the file name or size as well.
 *
 * @param file_name     the name of the transferred file.
 * @param total_read    the size of the information read.
 * @return int          0 in success, else otherwise.
 */
int receive_end_control_packet(char* file_name, long total_read);
