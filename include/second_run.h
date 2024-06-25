#ifndef ASSEMBLER_SECOND_RUN_H
#define ASSEMBLER_SECOND_RUN_H

void second_run();
char *return_instruction_line_in_binary(InstructionLine instruction_line);
bool is_instruction_line_directive(InstructionLine instructionLine);
bool is_instruction_line_opcode_command(InstructionLine instructionLine);
char *string_append(char *s1, char *s2);
char *get_opcode_binary_representation(int opcode, char* binary_string);
char *create_first_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string);
char *create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string);
bool instruction_line_has_three_binary_words(InstructionLine instructionLine);
char *return_instruction_line_in_binary(InstructionLine instruction_line);
#endif //ASSEMBLER_SECOND_RUN_H
