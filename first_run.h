#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

bool first_run(FILE *file, int *ic, int *dc);
void read_line(char *line);
int ignore_line(char *line);
char* skip_spaces(char *line);
bool find_label(const char *line, char **label);
bool is_macro(const char *line);
bool is_operand(const char *line);
const char *find_directive(const char *line);

#endif //ASSEMBLER_FIRST_RUN_H
