/*Non-Canonical Input Processing*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "serial_port.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;

int count = 0, success = 0;

void alarm_handler() {  // atende alarme
    printf("alarme # %d\n", count);
    success = 0;
    count++;
}

void read_answer(int fd, char *buf) {
    unsigned char answer_packet[5];
    success = 1;
    int i = 0;

    while (success) {               /* loop for input */
        int res = read(fd, buf, 1); /* returns after 1 char have been input */
        if (res < 0)
            continue;
        answer_packet[i] = buf[0];
        printf("%X\n", buf[0]);

        if (i > 0 && answer_packet[0] && buf[0] == answer_packet[0])  // FIXME
        {
            success = 1;
            break;
        }
        i++;
    }
    return;
}

int main(int argc, char **argv) {
    // int c;
    struct termios oldtio, newtio;
    char buf[255];
    // int sum = 0, speed = 0;

    if ((argc < 2) || ((strcmp("/dev/ttyS10", argv[1]) != 0) && (strcmp("/dev/ttyS11", argv[1]) != 0))) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        exit(1);
    }

    /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */

    int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        perror(argv[1]);
        exit(-1);
    }

    if (tcgetattr(fd, &oldtio) == -1) { /* save current port settings */
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

    if (tcsetattr(fd, TCSANOW, &newtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }

    printf("New termios structure set\n");

    (void)signal(SIGALRM, alarm_handler);

    // SET trama message
    unsigned char flag = HANDSHAKE_FLAG;
    unsigned char address = ADDRESS_SENDER_RECEIVER;
    unsigned char control = CONTROL_SET;
    unsigned char bcc = XOR(address, control);

    unsigned char request_packet[5] = {flag, address, control, bcc, flag};

    // receives the answer_packet
    while (count < 3) {
        if (success)
            break;

        // sends the set
        int res = write(fd, request_packet, sizeof(request_packet));
        alarm(3);  // activa alarme de 3s
        printf("%d bytes sent\n", res);

        read_answer(fd, buf);
    }

    sleep(1);
    if (tcsetattr(fd, TCSANOW, &oldtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }

    close(fd);
    return 0;
}