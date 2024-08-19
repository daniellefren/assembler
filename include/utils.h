#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H
#include "../include/structs.h"

/**
 * Convert a string to lowercase.
 * @param string - The string to be converted to lowercase.
 */
void lower_string(char *string);

/**
 * Trim leading and trailing spaces from a string.
 * @param str - The string to be trimmed.
 * @return A pointer to the trimmed string.
 */
char *trim_spaces(char *str);


char* itoa(int val, int base);

/**
 * Print the details of a command structure.
 * @param command - A pointer to the Command structure to print.
 */
void print_command(Command *command);

/**
 * Print the details of an instruction line structure.
 * @param instructionLine - A pointer to the InstructionLine structure to print.
 */
void print_instruction_line(InstructionLine *instructionLine);

/**
 * Print the details of a directive structure.
 * @param directive - A pointer to the Directive structure to print.
 */
void print_directive(Directive *directive);

/**
 * Erase the contents of a file by truncating it.
 * @param filename - The name of the file to erase.
 */
void erase_file_data(char *filename);

/**
 * Extract numeric characters, including negative numbers, from a string.
 * @param input - The input string to extract numbers from.
 * @param length - The length of the input string.
 * @return A pointer to the string containing extracted numbers.
 */
void extract_numbers(char *input, int length);

/**
 * Convert a string to an integer.
 * @param input - The string to convert.
 * @return The integer value of the string.
 */
int char_to_int(char *c);

/**
 * Checks if the directive is of type DATA.
 *
 * Determines whether the given directive is a data directive. Prints a message
 * if the directive type is DATA.
 *
 * @param directive Pointer to the Directive structure to check.
 * @return 1 if the directive type is DATA, 0 otherwise.
 */
int is_directive_data(Directive *directive);

/**
 * Checks if the directive is of type STRING.
 *
 * Determines whether the given directive is a string directive. Prints a message
 * if the directive type is STRING.
 *
 * @param directive Pointer to the Directive structure to check.
 * @return 1 if the directive type is STRING, 0 otherwise.
 */
int is_directive_string(Directive *directive);

/**
 * Converts a portion of a binary string to an octal string.
 *
 * Extracts a specified number of binary bits from the input `binary_string`, converts
 * it to an octal string, and stores the result in `octal_string`. The number of bits
 * must be a multiple of 3. Validates input parameters and prints diagnostic messages.
 *
 * @param binary_string The input string containing binary digits.
 * @param number_of_binary_bits The number of binary bits to convert.
 * @param offset The starting position in the binary string for extraction.
 * @param octal_string The buffer to store the resulting octal string.
 *
 */
void fill_octal_string_from_binary(char *binary_string, int number_of_binary_bits, int offset, char *octal_string);

/**
 * Converts a binary string to an octal string.
 *
 * Takes a binary string of fixed length `BINARY_WORD_LENGTH`, converts it to a decimal value,
 * and then to an octal representation. Stores the result in `octal_string`.
 *
 * @param binary_string The binary string to convert. Must be of length `BINARY_WORD_LENGTH`.
 * @param octal_string The buffer to store the resulting octal string.
 *
 */
void binary_to_octal(char *binary_string, char *octal_string);

/**
 * Converts an integer to a binary string representation.
 *
 * Converts the given integer `num` into a binary string with a specified number of bits.
 * The binary representation is stored in `binary_string` starting at the specified `offset`.
 * Negative numbers converted using the two's complement method
 *
 * @param num The integer value to convert.
 * @param binary_string The buffer to store the resulting binary string.
 * @param offset The starting position in `binary_string` for storing the result.
 * @param num_bits The number of bits to use for the binary representation (excluding sign bit).
 *
 */
void int_to_binary_string(int num, char *binary_string, int offset, int num_bits);

/**
 * Converts a character to a binary string representation.
 *
 * Converts the given character `c` into a binary string with a specified number of bits.
 * The binary representation is stored in `binary_string` starting at the specified `offset`.
 *
 * @param c The character to convert.
 * @param binary_string The buffer to store the resulting binary string.
 * @param offset The starting position in `binary_string` for storing the result.
 * @param num_bits The number of bits to use for the binary representation.
 *
 */
void char_to_binary_string(char c, char *binary_string, int offset, int num_bits);

/**
 * Check if a given string is a valid integer.
 * @param str - The string to check.
 * @return 0 if the string is not a valid integer, or the length of the integer if valid.
 */
int check_if_valid_integer(char *str);

/**
 * Skip leading spaces in a string.
 * @param line - The string to process.
 * @return A pointer to the first non-space character in the string.
 */
char* skip_spaces(char *line);

/**
 * Ignore a line if it is a comment (starting with ';') or empty.
 * @param line - The line to check.
 * @return 1 if the line should be ignored, otherwise 0.
 */
int ignore_line(char *line);

/**
 * Extract the word that follows a specific keyword in a string.
 * @param input - The input string to search.
 * @param output - The buffer to store the extracted word.
 * @param keyword - The keyword to search for in the input string.
 */
void extract_word_after_keyword(char *input, char *output, char *keyword);

/**
 * Remove the first character from a string.
 * @param str - The string from which the first character will be removed.
 */
void remove_first_character(char *str);

/**
 * Converts an integer to a string representation.
 * @param number - The integer value to convert to a string.
 * @param str - The character array (buffer) where the resulting string will be stored.
 *              Ensure that the buffer is large enough to hold the string representation
 *              of the number, including the null terminator.
 */
char* int_to_string(int number);

/**
 * Removes trailing newline and carriage return characters from a string.
 */
void strip_newline(char *line);


#endif
