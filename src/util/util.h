/**
 * @file    util.h
 * @brief   Defines useful flags and functions for daily C.
 * @date    2020-10-11
 */

#pragma once

/**
 * @brief Calculates a bitwise XOR between two numbers.
 */
#define XOR(first, second) (first ^ second)

/**
 * @brief 
 */
#define bool int

/**
 * @brief Defines the FALSE boolean.
 */
#define FALSE 0

/**
 * @brief Defines the TRUE boolean.
 */
#define TRUE 1

#define ESCAPE 0x7d

/**
 * @brief Checks the arguments of the programs, taking into account that each one receives no more than the port to
 * connect to each other.
 * 
 * @param argc  the number of the arguments
 * @param argv  the array of arguments
 * @return int  the error code or 0 if successful
 */
int check_arguments(int argc, char **argv);

/**
 * @brief 
 * 
 * @param data_size 
 * @param data 
 * @return unsigned char 
 */
unsigned char xor_array(int data_size, unsigned char *data);

// returns the same array if there's an error
void resize_array(unsigned char *array, unsigned size, unsigned char *res, unsigned first_index, unsigned res_size);

/**
 * @brief 
 * 
 * @param data 
 * @param data_size 
 * @param new_data 
 * @return int      the size of the new data array
 */
int stuff_data(unsigned char *data, int data_size, unsigned char *new_data);