/**
 * @file    util.h
 * @brief   Defines useful flags and functions for daily C.
 * @date    2020-10-11
 */

/**
 * @brief Calculates a bitwise XOR between two numbers.
 */
#define XOR(first, second) (first ^ second)

/**
 * @brief Defines the FALSE boolean.
 */
#define FALSE 0

/**
 * @brief Defines the TRUE boolean.
 */
#define TRUE 1

/**
 * @brief Checks the arguments of the programs, taking into account that each one receives no more than the port to
 * connect to each other.
 * 
 * @param argc  the number of the arguments
 * @param argv  the array of arguments
 * @return int  the error code or 0 if successful
 */
int check_arguments(int argc, char **argv);