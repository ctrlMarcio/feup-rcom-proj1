/**
 * @file    state_machine.h
 * @brief   Handles the state machines
 * @date    2020-10-13
 */

#pragma once

#include "serial_port.h"
#include "../../util/util.h"

enum set_state{START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, DATA, BCC2_OK, STOP};

bool update_state(enum set_state *state, char message, MessageConstruct construct);