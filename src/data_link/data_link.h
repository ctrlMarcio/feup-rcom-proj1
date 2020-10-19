/**
 * @file data_link.h
 * @brief 
 * @date 2020-10-16
 */

#pragma once

#include "util/util.h"

/**
 * @brief 
 * @param port 
 * @return int 
 */
int llopen(char *port, bool sender);

/**
 * @brief 
 * 
 * @param fd 
 * @return int 
 */
int llclose(int fd, bool sender);
