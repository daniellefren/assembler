#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"


void initMacroTable(MacroTable *table);
void addMacro(MacroTable *table, Macro macro);
bool findMacro(const MacroTable *table, const char *name, Macro *macro);
bool isMacroDefinitionStart(const char *line);
bool isMacroDefinitionEnd(const char *line);
bool isMacroInvocation(const char *line, char *macroName, char **macroNames);
int handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine);
void expandMacro(const Macro *macro, FILE *outputFile);
void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table);
void read_line(const char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro);
void addSymbol(SymbolTable *table, const char *label, int address, const char *type);
void handleInstruction(const char *line, int *ic);
int ignore_line(const char *line);
const char* skip_spaces(const char *line);
bool find_label(const char *line, char *label);
bool is_instruction(const char *line, int *ic);
bool symbolExists(const SymbolTable *table, const char *label) ;
bool is_directive(const char *line);
bool isDataDirective(const char *line);
bool isStringDirective(const char *line);
bool isExternDirective(const char *line);
void pre_run(const char *line, MacroTable *macroTable, char **macroNames, SymbolTable *symbol_table, FILE *file);

bool isEntryDirective(const char *line);

void handleDataDirective(const char *line, int *dc);
void handleStringDirective(const char *line, int *dc);
void initMacroNameArray(char **macroNames);

void writeExpandedMacrosToFile(MacroTable *table);
int write_line_to_file(const char *line);

#endif //ASSEMBLER_FIRST_RUN_H
