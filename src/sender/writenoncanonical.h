/**
 * @file    writenoncanonical.h
 * @brief   States important functions for non canonical writing.
 * @date    2020-10-11
 */

#include <termios.h>
#include <unistd.h>

/**
 * @brief Opens and configures the writing serial port.
 * @attention   The program will terminate here on error.
 * 
 * Open serial port device for reading and writing and not as controlling tty because we don't want to get killed if
 * linenoise sends CTRL-C.
 * 
 * @param port          the name of the serial port file
 * @param oldtio        the termios to save the old configuration
 * @return int          the fd of the opened port
 */
int open_writing_serial_port(char *port, struct termios *oldtio);

/**
 * @brief Constructs the set frame message.
 * 
 * @param set_frame    the built set message as an array, [FLAG, ADDRESS, CONTROL, BCC, FLAG]
 */
void define_set_frame(unsigned char *set_frame);

/**
 * @brief Contructs the message being sent
 * 
 * @param message the built message as an array
 * @return int 1 if success, 0 otherwise
 */
int define_message_frame(unsigned char *message, int control_setter, unsigned char* data);

/**
 * @brief Attempts to establish a connections through the port, using a SET message and expecting for an UA one.
 * 
 * @param fd            the fd of the port
 * @param set_frame    the SET message
 * @param attempts      the number of attempts
 * @param timeout       the time, in seconds, between attempts
 * @return int          1 if successfull, 0 otherwise
 */
int attempt_handshake(int fd, unsigned char *set_frame, int attempts, int timeout);

/**
 * @brief Sends the message
 * 
 * @param fd        // TODO
 * @param message the built message as an array
 * @return int
 */
int send_message(int fd, unsigned char *message);

/**
 * @brief Terminates the connection to the port.
 * 
 * @param fd            the fd of the port
 * @param oldtio        the old termios structure to reset
 * @return int          the error code in case of error, 0 on success
 */
int terminate_connection(int fd, struct termios *oldtio);

/**
 * @brief 
 * 
 * @param fd
 * @deprecated 
 */
void receive_message(int fd);

/**
 * @brief 
 * 
 * // TODO
 * 
 * @param fd 
 * @param control_setter 
 */
void receive_frame(int fd, int control_setter);