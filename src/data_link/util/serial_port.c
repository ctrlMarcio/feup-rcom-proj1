#include "serial_port.h"

char get_control(enum frame type, int sequence_number) {
    if (type == SET)
        return CONTROL_SET;
    if (type == UA)
        return CONTROL_UA;
    if (type == DISC)
        return CONTROL_DISC;
    if (sequence_number) {
        if (type == I)
            return CONTROL_I_ONE;
        if (type == RR)
            return CONTROL_RR_ONE;
        if (type == REJ)
            return CONTROL_REJ_ONE;    
    } else {
        if (type == I)
            return CONTROL_I_ZERO;
        if (type == RR)
            return CONTROL_RR_ZERO;
        if (type == REJ)
            return CONTROL_REJ_ZERO;
    }

    return 0;
}