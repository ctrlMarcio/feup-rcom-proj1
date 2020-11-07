/**
 * @file    state_machine.h
 * @brief   Handles the state machines.
 */

#pragma once

#include "serial_port.h"
#include "../../util/util.h"

/**
 * @brief The states a state machine can be in.
 */
enum set_state{START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, DATA, BCC2_OK, STOP};

/**
 * @brief Updates the state of the state machine, given the current state, a byte, and a construct.
 * 
 * @param state     the current state of the state machine.
 * @param message   the message byte received.
 * @param construct the construct that defines whats to be expected.
 * @return true     if the message was according to the construct.
 * @return false    otherwise.
 */
bool update_state(enum set_state *state, char message, MessageConstruct construct);