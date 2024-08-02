#ifndef ASSEMBLER_SECOND_RUN_H
#define ASSEMBLER_SECOND_RUN_H

void start_second_run(LinesArray *assembly_lines_array);
void fill_instruction_line_binary(InstructionLine *instruction_line);
bool is_instruction_line_directive(InstructionLine instructionLine);
bool is_instruction_line_opcode(InstructionLine instructionLine);
void set_binary_string_opcode_representation(int opcode, char* binary_string);
void create_first_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string, int size_of_binary);
void create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string, int size_of_binary);
bool instruction_line_has_three_binary_words(InstructionLine instructionLine);
char *return_instruction_line_in_binary(InstructionLine instruction_line);
void write_binary_lines_array_to_file(LinesArray *linesArray, FILE *fptr);
void fill_the_binary_representation_with_zero(char *binary_string, size_t length);
void set_binary_string_operand_representation(int first_operand_classification_type, int second_operand_classification_type, char *binary_string);
void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size);
void set_binary_string_ARE_representation(char *binary_string, int number_of_binary_string);

#endif //ASSEMBLER_SECOND_RUN_H
