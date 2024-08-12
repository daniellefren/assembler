
#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H
#include "../include/structs.h"
void lower_string(char *string);
char *trim_spaces(char *str);
char* itoa(int val, int base);
void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);
void print_command(Command *command);
void print_instruction_line(InstructionLine *instructionLine);
void print_directive(Directive *directive);
void erase_file_data(const char *filename);
char* extract_numbers(const char *input, int length);
int char_to_int(char *c);
void extract_word_after_keyword(const char *input, char *output, const char *keyword);
void remove_first_character(char *str);
bool is_directive_data(Directive *directive);
bool is_directive_string(Directive *directive);
void add_number_to_string(char *buffer, const char* source, size_t buffer_size, int number);
void fill_octal_string_from_binary(const char *binary_string, int number_of_binary_bits, int offset, char *octal_string);
void binary_to_octal(const char *binary_string, char *octal_string);
void int_to_binary_string(int num, char *binary_string, int offset, int num_bits);
void char_to_binary_string(char c, char *binary_string, int offset, int num_bits);
int check_if_valid_integer(char *str);
void add_output_directory();
#endif //ASSEMBLER_UTILS_H
