#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"


void add_macro(MacroTable *table, Macro macro);
int is_macro_definition_start(char *line);
int is_macro_definition_end(char *line);
int is_macro_invocation(char *line, char *macroName, char **macroNames);
int handle_macro_definition(FILE *file, MacroTable *macroTable, const char *firstLine);
void expand_macro(const Macro *macro, FILE *outputFile);
void get_expanded_macro_file_name(char *buffer, size_t buffer_size, int number);
void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table, int file_number);
int read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, LinesArray *lines_array, MacroTable *macro_table, int file_number);
int pre_run(MacroTable *macro_table, char **macroNames, FILE *file, char* new_file_name);
void handle_string_directive(char *line, Directive *new_directive, InstructionLine *instruction_line);
void handle_data_directive(char *line, Directive *new_directive, InstructionLine *instruction_line);


int handle_command(char *line, SymbolTable *symbol_table, MacroTable *macro_table, InstructionLine *new_instruction_line);
void handle_directives(char *line, int *dc, SymbolTable *symbol_table, int* ic, int file_number, InstructionLine *new_instruction_line);
Symbol* handle_symbol(InstructionLine *instruction_line, char *symbol_name);
void handle_extern_directive(char *line, Directive *new_directive, SymbolTable *symbol_table, int file_number, int *ic);
void handle_entry_directive(Directive *new_directive, int file_number, SymbolTable *symbol_table, char* line);
int classify_operand(Operand *new_operand);
int find_number_of_lines_in_binary(Command *new_command);

Symbol *find_symbol_by_name(SymbolTable* symbol_table, char* symbol_name);

int ignore_line(char *line);
char* skip_spaces(char *line);

int find_symbol(char *line, char *symbol);
int symbol_exists(SymbolTable *symbol_table, char *symbol); // TODO - check if relevant
int is_directive(char *line);
int is_command(char *line);
int is_valid_symbol(const char *symbol, MacroTable *macro_table);

int get_operand_opcode(char *command_name);


void get_operands_data_for_command(char* command_name, Command *new_command);
void define_operands_from_line(Command *new_command, char* line);

int define_operand_types(Operand *operand, MacroTable *macro_table);

int is_known_assembly_keyword(const char *symbol);

void extract_second_operand_from_line(char* line, Command *new_command);

#endif //ASSEMBLER_FIRST_RUN_H
