/*Non-Canonical Input Processing*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "serial_port.h"

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;

int main(int argc, char **argv)
{
    // int c;
    struct termios oldtio, newtio;
    char buf[255];

    if ((argc < 2) || ((strcmp("/dev/ttyS10", argv[1]) != 0) && (strcmp("/dev/ttyS11", argv[1]) != 0)))
    {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        exit(1);
    }

    /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
    */

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror(argv[1]);
        exit(-1);
    }

    if (tcgetattr(fd, &oldtio) == -1)
    { /* save current port settings */
        perror("tcgetattr");
        exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;  /* blocking read until 5 chars received */

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) == -1)
    {
        perror("tcsetattr");
        exit(-1);
    }

    printf("New termios structure set\n");

    unsigned char request_packet[5];
    int res;

    unsigned int i = 0;
    while (STOP == FALSE)
    {                           /* loop for input */
        res = read(fd, buf, 1); /* returns after 1 char have been input */
        request_packet[i] = buf[0];
        printf("%X\n", buf[0]);
        if (i > 0 && request_packet[0] && buf[0] == request_packet[0]) // FIXME TEMP state machine
            STOP = TRUE;
        i++;
    }

    // UA
    unsigned char flag = HANDSHAKE_FLAG;
    unsigned char address = ADDRESS_SENDER_RECEIVER;
    unsigned char control = CONTROL_UA;
    unsigned char bcc = XOR(address, control);

    unsigned char answer_packet[5] = {flag, address, control, bcc, 0x00};

    // sends the answer
    res = write(fd, answer_packet, sizeof(answer_packet));
    printf("%d bytes sent\n", res);

    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
    return 0;
}