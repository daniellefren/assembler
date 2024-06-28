#define _STD_C99 1

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/structs.h"
#include "../include/first_run.h"
#include "../include/assembler.h"
#include "../include/constants.h"

char macroFileName[] = "expanded_macros.txt";


void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table) {
    char line[MAX_LINE_LENGTH];
    char *macroNames[MAX_MACRO_NAMES];  // Array to store pointers to macro names
    int line_num = 1;
    int is_in_macro = 0;
    MacroTable macroTable;

    initMacroTable(&macroTable);
    initMacroNameArray(macroNames);

    // Reset file pointer to the beginning before calling pre_run
    rewind(file);
    pre_run(line, &macroTable, macroNames, symbol_table, file); // Keeps track of the number of encountered macros

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, symbol_table, ic, dc, is_in_macro);
        }
        printf("Line: %s\n", line);
        line_num++;
    }

    // Free allocated memory for macro names
    for (int j = 0; j < MAX_MACRO_NAMES; ++j) {
        free(macroNames[j]);
    }
}

void pre_run(const char *line, MacroTable *macroTable, char **macroNames, SymbolTable *symbol_table, FILE *file) {
    // Run and expand all macros in the program.
    // Return number of macros
    int is_in_macro = 0;
    int macroCount = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char macroName[MAX_LABEL_LENGTH];
        if (!ignore_line(line)) {
            if (isMacroDefinitionStart(line)) {
                is_in_macro = 1;
                printf("Macro definition start\n");

                sscanf(line, "%*s %s", macroName);  // Skip "%macro" and capture the name
                printf("Macro name: %s\n", macroName);

                if (macroCount < MAX_MACRO_NAMES) {
                    strcpy(macroNames[macroCount++], macroName);
                } else {
                    fprintf(stderr, "Warning: Reached maximum number of macro names (%d)\n", MAX_MACRO_NAMES);
                }
                is_in_macro = handleMacroDefinition(file, macroTable, line); // Pass macroTable directly
                printf("Macro handling done, is_in_macro: %d\n", is_in_macro);
            } else if (isMacroInvocation(line, macroName, macroNames)) {
                printf("Macro invocation: %s\n", macroName);
                Macro macro;
                printf("it is a macro");
                expandMacro(&macro, stdout);
                writeExpandedMacrosToFile(macroTable);

            } else {
                printf("Not a macro invocation: %s\n", line);
                if (!is_in_macro) {
                    write_line_to_file(line);
                }
            }
        }
    }
}

void read_line(const char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro) {
    const char *directive;
    char label[MAX_LABEL_LENGTH] = {0};
    int hasLabel;

    line = skip_spaces(line);

    hasLabel = find_label(line, label);
    if (hasLabel) {
        line = strchr(line, ':') + 1;
    }

    if (is_directive(line)) {
        if (hasLabel) {
            addSymbol(symbol_table, label, *dc, directive);
        }
        if (isDataDirective(line)) {
            printf("Data directive: %s\n", directive);
            handleDataDirective(line, dc);
        } else if (isStringDirective(line)) {
            printf("String directive: %s\n", directive);
            handleStringDirective(line, dc);
        } else if (is_instruction(line, ic)) {
            handleInstruction(line, ic);
        }
    }
}

int write_line_to_file(const char *line) {
    FILE *outputFile = fopen(macroFileName, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n", macroFileName);
        return 1;  // Indicate failure
    }

    // Write the string to the file
    fprintf(outputFile, "%s", line);
    printf("Wrote: %s", line);

    fclose(outputFile);
    return 0;  // Indicate success
}

void writeExpandedMacrosToFile(MacroTable *macroTable) {
    FILE *outputFile = fopen(macroFileName, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing macros\n", macroFileName);
        return;
    }

    // Write each macro's body to the file
    for (int i = 0; i < macroTable->count; ++i) {
        for (int j = 0; j < macroTable->macros[i].lineCount; ++j) {
            fprintf(outputFile, "%s", macroTable->macros[i].body[j]);
        }
    }

    fclose(outputFile);
    printf("Successfully wrote expanded macros to '%s'\n", macroFileName);
}

void initMacroTable(MacroTable *table) {
    table->macros = (Macro *)malloc(10 * sizeof(Macro));
    if (!table->macros) {
        fprintf(stderr, "Error: Memory allocation failed for macro table\n");
        exit(EXIT_FAILURE);
    }
    table->count = 0;
    table->capacity = 10;
}

// Initialize macro name array
void initMacroNameArray(char **macroNames) {
    for (int i = 0; i < MAX_MACRO_NAMES; ++i) {
        macroNames[i] = malloc(MAX_LABEL_LENGTH * sizeof(char));
        if (!macroNames[i]) {
            fprintf(stderr, "Error: Memory allocation failed for macro name %d\n", i);
            exit(EXIT_FAILURE);
        }
        macroNames[i][0] = '\0'; // Ensure the string is initialized
    }
}

void addMacro(MacroTable *table, Macro macro) {
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->macros = (Macro *)realloc(table->macros, table->capacity * sizeof(Macro));
        if (!table->macros) {
            fprintf(stderr, "Error: Memory allocation failed while expanding macro table\n");
            exit(EXIT_FAILURE);
        }
    }
    table->macros[table->count++] = macro;
}

int findMacro(const MacroTable *table, const char *name, Macro *macro) {
    for (int i = 0; i < table->count; ++i) {
        if (strcmp(table->macros[i].name, name) == 0) {
            *macro = table->macros[i];
            return 1;
        }
    }
    return 0;
}

bool isMacroDefinitionStart(const char *line) {
    return strstr(line, "macr ") != NULL;
}

bool isMacroDefinitionEnd(const char *line) {
    return strstr(line, "endmacr") != NULL;
}

int isMacroInvocation(const char *line, char *macroName, char **macroNames) {
    printf("Check if is macro invoke: %s\n", line);

    // Extract potential macro name
    sscanf(line, "%s", macroName);

    // Check if the extracted name is not empty and exists in macroNames
    if (macroName[0] != '\0') {
        for (int i = 0; i < MAX_MACRO_NAMES && macroNames[i][0] != '\0'; ++i) {
            if (strcmp(macroName, macroNames[i]) == 0) {
                printf("Macro invocation: %s\n", macroName);
                return 1; // Macro name found in the array
            }
        }
    }

    return 0; // Not a macro invocation or name not found
}

// Return 0 if ended, else 1
int handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine) {
    printf("Handle macro definition\n");
    Macro macro;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LABEL_LENGTH];

    sscanf(firstLine, "%s", macroName);
    strcpy(macro.name, macroName + 7);  // Skip "%macro"
    macro.lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isMacroDefinitionEnd(line)) {
            addMacro(macroTable, macro);
            printf("Macro ended successfully\n");
            return 0;
        }
        strcpy(macro.body[macro.lineCount++], line);
    }

    addMacro(macroTable, macro);
    return 1;
}

void expandMacro(const Macro *macro, FILE *outputFile) {
    for (int i = 0; i < macro->lineCount; ++i) {
        fprintf(outputFile, "%s", macro->body[i]);
    }
}

void addSymbol(SymbolTable *table, const char *label, int address, const char *type) {
    if (symbolExists(table, label)) {
        fprintf(stderr, "Error: Symbol '%s' already exists\n", label);
        return;
    }

    if (table->size >= table->capacity) {
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, table->capacity * sizeof(Symbol));
        if (!table->symbols) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    strcpy(table->symbols[table->size].label, label);
    table->symbols[table->size].address = address;
    strcpy(table->symbols[table->size].type, type);
    table->size++;
}

void handleInstruction(const char *line, int *ic) {
    printf("Instruction: %s\n", line);
    (*ic)++;
}

int ignore_line(const char *line) {
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

const char* skip_spaces(const char *line) {
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

// Function to check if a line is a label
int find_label(const char *line, char *label) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    const char *start = line;

    while (*line && !isspace((unsigned char)*line) && *line != ':') {
        line++;
    }

    if (*line == ':') {
        size_t len = line - start;
        strncpy(label, start, len);
        label[len] = '\0'; // Null-terminate the label
        return 1;
    }

    return 0;
}

// Function to check if a line is an instruction
int is_instruction(const char *line, int *ic) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strncmp(line, commands[i], strlen(commands[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

int symbolExists(const SymbolTable *table, const char *label) {
    for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->symbols[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a line is a directive
bool is_directive(const char *line) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return *line == '.';
}

bool isDataDirective(const char *line) {
    return strstr(line, ".data") != NULL;
}

bool isStringDirective(const char *line) {
    return strstr(line, ".string") != NULL;
}

bool isExternDirective(const char *line) {
    return strstr(line, ".extern") != NULL;
}

bool isEntryDirective(const char *line) {
    return strstr(line, ".entry") != NULL;
}

void handleDataDirective(const char *line, int *dc) {
    // Implementation here
}

void handleStringDirective(const char *line, int *dc) {
    // Implementation here
}
