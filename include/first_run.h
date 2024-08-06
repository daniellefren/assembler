#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"


void add_macro(MacroTable *table, Macro macro);
bool is_macro_definition_start(char *line);
bool is_macro_definition_end(char *line);
int is_macro_invocation(char *line, char *macroName, char **macroNames);
int handle_macro_definition(FILE *file, MacroTable *macroTable, const char *firstLine);
void expand_macro(const Macro *macro, FILE *outputFile);
void write_expanded_macros_to_file(MacroTable *table);

void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, LabelTable *label_table);
void read_line(char *line, LabelTable *label_table, int *ic, int *dc, LinesArray *lines_array);
void pre_run(char *line, MacroTable *macroTable, char **macroNames, FILE *file);

void handle_command(char *line, Command *new_command, LabelTable *label_table);
void handle_directives(char *line, int *dc, Directive *new_directive, LabelTable *label_table, int* ic);
void classify_operand(Operand *new_operand);
int find_number_of_lines_in_binary(Command *new_command);
//void handle_data_directive(char* directive_type, Directive *new_directive, char* ptr, char *values[MAX_LINE_LENGTH], int *dc, int *values_count);
void add_extern_to_externals_file();
int ignore_line(char *line);
char* skip_spaces(char *line);

int find_label(char *line, char *label);
int label_exists(LabelTable *label_table, char *label); // TODO - check if relevant
bool is_data_directive(char *line);
bool is_extern_directive(char *line);
int is_command(char *line, int *ic);
int is_valid_label(const char *label);
bool is_entry_directive(char *line);
void define_operand_types(Operand *operand, LabelTable *label_table);

int get_operand_opcode(char *command_name);


int write_line_to_file(char *line);
void get_command_data(char* command_name, Command *new_command);
void define_operands_from_line(Command *new_command, char* line);

void define_operand_types(Operand *operand, LabelTable *label_table);

int is_known_assembly_keyword(const char *label);

void debugging_data(Directive *new_directive);
void extract_second_operand_from_line(char* line, Command *new_command);

#endif //ASSEMBLER_FIRST_RUN_H
