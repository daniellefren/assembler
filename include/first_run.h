#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array);
void read_line(const char *line);
int ignore_line(char *line);
const char* skip_spaces(const char *line);
bool find_label(const char *line, char *label);
bool is_macro(const char *line);
bool is_operand(const char *line);
const char *find_directive(const char *line);

#endif //ASSEMBLER_FIRST_RUN_H
