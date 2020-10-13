#include "state_machine.h"

#include "util.h"

void update_state(enum set_state *state, char message, MessageConstruct construct) {
    switch(*state) {
        case START:
            if (message == construct.flag)
                *state = FLAG_RCV;
            break;

        case FLAG_RCV:
            if (message == construct.address)
                *state = A_RCV;
            else if (message != construct.flag)
                *state = START;
            break;

        case A_RCV:
            if (message == construct.control)
                *state = C_RCV;
            else if (message == construct.flag)
                *state = FLAG_RCV;
            else
                *state = START;
            break;

        case C_RCV:
            if (message == XOR(construct.address, construct.control))
                *state = BCC_OK;
            else if (message == construct.flag)
                *state = FLAG_RCV;
            else
                *state = START;
            break;

        case BCC_OK:
            if (message == construct.flag)
                *state = STOP;
            else
                *state = START;
            break;

        default:
            break;
    }
}

/* TODO for flavia to decide which one she prefers 🥺
void update_state(enum set_state *state, char message, MessageConstruct construct) {
    if (*state == START && message == construct.flag)
        *state = FLAG_RCV;
    else if (*state == FLAG_RCV && message == construct.address)
        *state = A_RCV;
    else if (*state == A_RCV && message == construct.control)
        *state = C_RCV;
    else if (*state == C_RCV && message == XOR(construct.address, construct.control))
        *state = BCC_OK;
    else if (*state == BCC_OK && message == construct.flag)
        *state = STOP;
    else if (*state == STOP)
        *state = STOP;
    else if (message == construct.flag)
        *state = FLAG_RCV;
    else
        *state = START;
}
*/