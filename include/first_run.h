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
void read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro, LinesArray *lines_array);
void handleCommand(char *line, int *ic, LinesArray *lines_array, SymbolTable *symbol_table);
int ignore_line(char *line);
char* skip_spaces(char *line);
int find_label(char *line, char *label);
int is_command(char *line, int *ic);
int symbolExists(SymbolTable *table, char *label);
bool isDataDirective(char *line);
bool isExternDirective(char *line);
void pre_run(char *line, MacroTable *macroTable, char **macroNames, SymbolTable *symbol_table, FILE *file);
int get_operand_opcode(char *command_name);
bool isEntryDirective(char *line);

void handleDataDirective();
void handleStringDirective();
void initMacroNameArray(char **macroNames);

void writeExpandedMacrosToFile(MacroTable *table);
int write_line_to_file(char *line);
void getCommandData(char* command_name, InstructionLine *instruction_line);
void defineOperandsFromLine(int operand_number_value, char *first_operand, char *second_operand, char* line);
void classify_operand(const char *operand, int *operand_type);
void handleDirectives(char *line, int *dc, SymbolTable *symbol_table, Symbol *new_symbol);
int checkIfOperandLabel(char *operand, SymbolTable *symbol_table);
void debuggingData(Symbol *new_symbol);
#endif //ASSEMBLER_FIRST_RUN_H
