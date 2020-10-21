#include "util.h"

#include <stdio.h>

long to_bytes(int amount, enum unit_measure unit) {
    long value = (long) amount;

    for (enum unit_measure tmp = unit; tmp > 0; tmp--)
        value *= 1024;
        
    return value;
}