#include "state_machine.h"

#include "util.h"

//TODO : Update state machine to receive data
void update_state(enum set_state *state, char message, MessageConstruct construct) {
    switch (*state) {
        case START:
            if (message == FRAME_FLAG)
                *state = FLAG_RCV;
            break;

        case FLAG_RCV:
            if (message == construct.address)
                *state = A_RCV;
            else if (message != FRAME_FLAG)
                *state = START;
            break;

        case A_RCV:
            if (message == construct.control)
                *state = C_RCV;
            else if (message == FRAME_FLAG)
                *state = FLAG_RCV;
            else
                *state = START;
            break;

        case C_RCV:
            if (message == XOR(construct.address, construct.control))
                *state = BCC_OK;
            else if (message == FRAME_FLAG)
                *state = FLAG_RCV;
            else
                *state = START;
            break;

        case BCC_OK:
            if (construct.data == FALSE) {
                if (message == FRAME_FLAG)
                    *state = STOP;
                else
                    *state = START;
            } else {
                *state = DATA;
            }
            break;

        case DATA:
            if (message == FRAME_FLAG)  // FIXME stuffing
                *state = STOP;
            break;

        default:
            break;
    }
}

// 😍