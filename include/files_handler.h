
#ifndef ASSEMBLER_FILES_HANDLER_H
#define ASSEMBLER_FILES_HANDLER_H
#include "../include/utils.h"

void add_extern_to_externals_file(Symbol *symbol, int file_number, int *ic);
void add_entry_to_entries_file(char *symbol, int file_number, int symbol_address);
void add_first_line_to_ob_file(int number_of_command, int number_of_directive, FILE *object_file);
int write_line_to_file(char *line, char* new_file_name);
void write_expanded_macros_to_file(MacroTable *macro_table, char* new_file_name);
FILE* open_ob_file(char *ob_file_name);
void create_ob_file(LinesArray *linesArray, int file_number);
void add_all_command_lines_to_ob_file(LinesArray *lines_array, FILE *object_file);
void add_all_directive_lines_to_ob_file(LinesArray *lines_array, FILE *object_file);
void add_command_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file);
void add_directive_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file);
#endif //ASSEMBLER_FILES_HANDLER_H
