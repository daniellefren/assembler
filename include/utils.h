
#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

void lower_string(char *string);
char *trim_spaces(char *str);
char* itoa(int val, int base);
void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);
//void free_binary_instruction(InstructionLine *p_line);
//void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length);
#endif //ASSEMBLER_UTILS_H
