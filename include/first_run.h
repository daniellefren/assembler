#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"


void initMacroTable(MacroTable *table);
void addMacro(MacroTable *table, Macro macro);
int findMacro(const MacroTable *table, const char *name, Macro *macro);
bool isMacroDefinitionStart(char *line);
bool isMacroDefinitionEnd(char *line);
int isMacroInvocation(char *line, char *macroName, char **macroNames);
int handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine);
void expandMacro(const Macro *macro, FILE *outputFile);
void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table);
void read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro);
void addSymbol(SymbolTable *table, const char *label, int address, const char *type);
void handleCommand(char *line, int *ic);
int ignore_line(char *line);
char* skip_spaces(char *line);
int find_label(char *line, char *label);
int is_command(char *line, int *ic);
int symbolExists(SymbolTable *table, char *label);
bool is_directive(char *line);
bool isDataDirective(char *line);
bool isStringDirective(char *line);
bool isExternDirective(char *line);
void pre_run(char *line, MacroTable *macroTable, char **macroNames, SymbolTable *symbol_table, FILE *file);
int get_operand_opcode(char *command_name);
bool isEntryDirective(char *line);

void handleDataDirective(char *line, int *dc);
void handleStringDirective(char *line, int *dc);
void initMacroNameArray(char **macroNames);

void writeExpandedMacrosToFile(MacroTable *table);
int write_line_to_file(char *line);
int get_operands_number_for_command(int command_opcode);
void define_operands_from_line(int operand_number_value, char *first_operand, char *second_operand, char* line);
void classify_operand(const char *operand, int *operand_type);

#endif //ASSEMBLER_FIRST_RUN_H
