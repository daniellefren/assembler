#ifndef ASSEMBLER_SECOND_RUN_H
#define ASSEMBLER_SECOND_RUN_H

void start_second_run(LinesArray *assembly_lines_array);
char *return_instruction_line_in_binary(InstructionLine instruction_line);
bool is_instruction_line_directive(InstructionLine instructionLine);
bool is_instruction_line_opcode_command(InstructionLine instructionLine);
char *get_opcode_binary_representation(int opcode, char* binary_string);
char *create_first_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string);
char *create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string);
bool instruction_line_has_three_binary_words(InstructionLine instructionLine);
char *return_instruction_line_in_binary(InstructionLine instruction_line);
void write_binary_lines_array_to_file(LinesArray *linesArray, FILE *fptr);


void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);


#endif //ASSEMBLER_SECOND_RUN_H
