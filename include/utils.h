
#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H
#include "../include/structs.h"
#include <stdbool.h>
void lower_string(char *string);
char *trim_spaces(char *str);
void erase_file_data(const char *filename);
char* extract_numbers(const char *input, int length);
void extract_word_after_keyword(const char *input, char *output, const char *keyword);
void remove_first_character(char *str);
void add_number_to_string(char *buffer, const char* source, size_t buffer_size, int number);
int check_if_valid_integer(char *str);
void add_output_directory();

/**
 * Appends two strings into a pre-allocated buffer.
 *
 * Concatenates `first_string` and `second_string`, storing the result in `appended_string`.
 * Ensures that the buffer `appended_string` is large enough to hold the combined strings
 * plus the null terminator. If the buffer is too small, an error message is printed and
 * the program exits.
 *
 * @param first_string The first string to append.
 * @param second_string The second string to append.
 * @param appended_string The buffer to store the concatenated result.
 * @param appended_string_size The size of the buffer `appended_string`.
 *
 */
void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);

/**
 * Prints the details of a Command structure.
 *
 * Displays the command's name, source operand (if present), and destination operand (if present).
 *
 * @param command Pointer to the Command structure to be printed.
 */
void print_command(Command *command);

/**
 * Prints the details of an InstructionLine structure.
 *
 * Displays the content of the line, its starting address, and prints additional
 * information depending on whether it contains a command or a directive.
 * Calls `print_command` if a command is present, or `print_directive` if a directive is present.
 *
 * @param instructionLine Pointer to the InstructionLine structure to be printed.
 */
void print_instruction_line(InstructionLine *instructionLine);

/**
 * Prints the details of a Directive structure.
 *
 * Displays the length of the directive and its values. If the directive contains data values,
 * all values are printed. If it contains a single value, that value is printed.
 *
 * @param directive Pointer to the Directive structure to be printed.
 */
void print_directive(Directive *directive);

/**
 * Converts a string representation of an integer to an integer value.
 *
 * Parses the input string `str` to convert it into an integer. Handles optional
 * leading negative sign to support negative numbers. Assumes that the string
 * contains valid integer characters (digits) and may start with a '-' for negative values.
 *
 * @param str Pointer to the null-terminated string to convert.
 * @return The integer value represented by the string.
 */
int char_to_int(char *c);

/**
 * Checks if the directive is of type DATA.
 *
 * Determines whether the given directive is a data directive. Prints a message
 * if the directive type is DATA.
 *
 * @param directive Pointer to the Directive structure to check.
 * @return True if the directive type is DATA, false otherwise.
 */
bool is_directive_data(Directive *directive);

/**
 * Checks if the directive is of type STRING.
 *
 * Determines whether the given directive is a string directive. Prints a message
 * if the directive type is STRING.
 *
 * @param directive Pointer to the Directive structure to check.
 * @return True if the directive type is STRING, false otherwise.
 */
bool is_directive_string(Directive *directive);

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
void fill_octal_string_from_binary(const char *binary_string, int number_of_binary_bits, int offset, char *octal_string);

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
void binary_to_octal(const char *binary_string, char *octal_string);

/**
 * Converts an integer to a binary string representation.
 *
 * Converts the given integer `num` into a binary string with a specified number of bits.
 * The binary representation is stored in `binary_string` starting at the specified `offset`.
 * The string includes a sign bit (0 for positive, 1 for negative).
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

#endif //ASSEMBLER_UTILS_H
