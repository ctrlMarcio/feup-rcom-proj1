/**
 * @file    util.h
 * @brief   Defines useful flags and functions for daily C.
 */

#pragma once

#include <time.h>
#include <stdio.h>

/**
 * @brief Calculates a bitwise XOR between two numbers.
 */
#define XOR(first, second) (first ^ second)

/**
 * @brief Defines a boolean through an int.
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

/**
 * @brief Defines the escape character.
 */
#define ESCAPE 0x7d

/**
 * @brief Defines entities, sender and receiver.
 * 
 */
enum entity {
    SENDER,
    RECEIVER
};

/**
 * @brief Defines units of software measure.
 */
enum unit_measure {B, KB, MB, GB};

/**
 * @brief Verifies the sender program arguments.
 * 
 * @param argc      the number of arguments.
 * @param argv      the array of arguments as strings.
 * @param filename  the returing string with the name of the file to send.
 * @return int      the boolean that states if the program should use or not virtual ports.
 */
int check_sender_arguments(int argc, char **argv, char **filename);

/**
 * @brief Verifies the receiver program arguments.
 * 
 * @param argc      the number of arguments.
 * @param argv      the array of arguments as strings.
 * @return int      the boolean that states if the program should use or not virtual ports.
 */
int check_receiver_arguments(int argc, char **argv);

/**
 * @brief Calculates a xor throughout all elements of an array of chars (bytes).
 * 
 * @param data_size         the size of the array.
 * @param data              the array.
 * @return unsigned char    the result of the XOR.
 */
unsigned char xor_array(int data_size, char *data);

/**
 * @brief Trims an array to another array.
 * 
 * @param array         the array to trim.
 * @param size          the size of the array to trim.
 * @param res           the resulting array. Equal to the first one in case of error.
 * @param first_index   the index of the array to starting trimming on.
 * @param res_size      the size of the resulting array.
 */
void resize_array(char *array, unsigned size, char *res, unsigned first_index, unsigned res_size);

/**
 * @brief Stuffs the information of an array.
 * 
 * @param data          the data array to stuff.
 * @param data_size     the size of the array to stuff.
 * @param new_data      the resuling array.
 * @return int          the size of the new array.
 */
int stuff_data(char *data, int data_size, char *new_data);

/**
 * @brief Appends an array to another one. The original array holds the result so it should be big enough
 * 
 * @param original          the first array, to append to as well.
 * @param size              the size of the first array.
 * @param to_append         the array to append.
 * @param to_append_size    the size of the array to append.
 * @return int              the new size of the array.
 */
int append_array(char *original, int size, char* to_append, int to_append_size);

/**
 * @brief Calculates the size of a file in bytes.
 * 
 * @param file_name     the name of the file.
 * @return long         the size of the file.
 */
long get_file_size(char *filename);

/**
 * @brief Asks through the terminal if the file should be overwritten.
 * 
 * @param filename      the name of the file.
 * @return true         if the user answers affirmatively.
 * @return false        otherwise.
 */
bool replace_file(char *filename);

/**
 * @brief Prints the progress of the file transfering in a beautiful way.
 * 
 * @param total_read    the size of data already done.
 * @param size          the total size.
 * @param start         the start time.
 */
void print_progess(int total_read, int size, clock_t start);

/**
 * @brief Translates a given size to bytes.
 * 
 * @param amount        the size.
 * @param unit          the measure unit.
 * @return long         the result in bytes.
 */
long to_bytes(int amount, enum unit_measure unit);

/**
 * @brief Calculates the number of octets needed to represent a number.
 * 
 * @param total_bytes       the number to represent.
 * @return unsigned char    the number of octets needed to represent the number.
 */
unsigned char amount_octets(long total_bytes);

/**
 * @brief Transforms a number into an array of octets.
 * 
 * @param file_size         the number.
 * @param size_in_octets    the resuling array of octets.
 * @param arr_size          the size of the array resulting array.
 */
void size_in_octets(long file_size, char* size_in_octets, unsigned arr_size);

/**
 * @brief Transforms an array of octets into a number.
 * 
 * @param octets            the array of octets.
 * @param octets_length     the size of the array.
 * @return long             the resulting number.
 */
long octets_to_size(unsigned char* octets, int octets_length);