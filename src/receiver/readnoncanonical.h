/**
 * @file readnoncanonical.h
 * @brief 
 * @date 2020-10-11
 */

#include <termios.h>
#include <unistd.h>

/**
 * @brief Opens and configures the reading serial port.
 * @attention   The program will terminate here on error.
 * 
 * Open serial port device for reading and writing and not as controlling tty because we don't want to get killed if
 * linenoise sends CTRL-C.
 * 
 * @param port          the name of the serial port file
 * @param oldtio        the termios to save the old configuration
 * @return int          the fd of the opened port
 */
int open_reading_serial_port(char *port, struct termios *oldtio);

/**
 * @brief Reads the sent packet sent by the sender.
 * 
 * @param fd            the fd of the port
 */
void read_set_packet(int fd);

/**
 * @brief Constructs the UA packet message.
 * 
 * @param set_packet    the built UA message as an array, [FLAG, ADDRESS, CONTROL, BCC, FLAG]
 */
void define_ua_packet(unsigned char *ua_packet);

/**
 * @brief Answers with the UA packet
 * 
 * @param ua_packet     the UA packet
 * @param fd            the port's fd
 */
void send_ua_packet(unsigned char *ua_packet, int fd);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 */
void terminate_connection(int fd, struct termios *oldtio);