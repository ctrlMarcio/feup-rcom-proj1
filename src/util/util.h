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
unsigned char xor_array(int data_size, char *data);

// returns the same array if there's an error
void resize_array(char *array, unsigned size, char *res, unsigned first_index, unsigned res_size);

/**
 * @brief 
 * 
 * @param data 
 * @param data_size 
 * @param new_data 
 * @return int      the size of the new data array
 */
int stuff_data(char *data, int data_size, char *new_data);

/**
 * @brief Appends an array to another one. The original array holds the result so it should be big enough
 * 
 * @param original 
 * @param size 
 * @param to_append 
 * @param to_append_size 
 * @return int      the new size of the array
 */
int append_array(char *original, int size, char* to_append, int to_append_size);