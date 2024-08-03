#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"


void addMacro(MacroTable *table, Macro macro);
int findMacro(const MacroTable *table, const char *name, Macro *macro);
bool isMacroDefinitionStart(char *line);
bool isMacroDefinitionEnd(char *line);
int isMacroInvocation(char *line, char *macroName, char **macroNames);
int handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine);
void expandMacro(const Macro *macro, FILE *outputFile);
int first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, LabelTable *label_table);
int read_line(char *line, LabelTable *label_table, int *ic, int *dc, LinesArray *lines_array);
void handleCommand(char *line, int *ic, Command *new_command, LabelTable *label_table);
int ignore_line(char *line);
char* skip_spaces(char *line);
int find_label(char *line, char *label, int* return_value);
int is_command(char *line, int *ic);
int labelExists(LabelTable *label_table, char *label);
bool isDataDirective(char *line);
bool isExternDirective(char *line);
void pre_run(char *line, MacroTable *macroTable, char **macroNames, FILE *file, int *return_value);
int get_operand_opcode(char *command_name);
bool isEntryDirective(char *line);

void handleDataDirective();
void handleStringDirective();
void initMacroNameArray(char **macroNames);

void writeExpandedMacrosToFile(MacroTable *table);
int write_line_to_file(char *line);
void getCommandData(char* command_name, Command *new_command);
void defineOperandsFromLine(Command *new_command, char* line);
void classify_operand(Operand *new_operand);
void handleDirectives(char *line, int *dc, Directive *new_directive);
int checkIfOperandLabel(char *operand, LabelTable *label_table);
void debuggingData(Directive *new_directive);
void defineOperandTypes(Operand *operand, LabelTable *label_table);

int find_number_of_lines_in_binary(Command *new_command);
int is_known_assembly_keyword(const char *label);

#endif //ASSEMBLER_FIRST_RUN_H
