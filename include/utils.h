
#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H
#include "../include/structs.h"
void lower_string(char *string);
char *trim_spaces(char *str);
char* itoa(int val, int base);
void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);
void free_binary_instruction(InstructionLine *p_line);
void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length);
void print_command(Command *command);
void print_instruction_line(InstructionLine *instructionLine);
void print_directive(Directive *directive);
void erase_file_data(const char *filename);
#endif //ASSEMBLER_UTILS_H
