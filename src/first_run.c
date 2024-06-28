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
#include "../include/utils.h"

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

    FILE *expanded_macros_file = fopen(macroFileName, "r");

    while (fgets(line, MAX_LINE_LENGTH, expanded_macros_file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, symbol_table, ic, dc, is_in_macro);
        }
        line_num++;
    }

    fclose(file);

    // Free allocated memory for macro names
    for (int j = 0; j < MAX_MACRO_NAMES; ++j) {
        free(macroNames[j]);
    }
}

void pre_run(char *line, MacroTable *macroTable, char **macroNames, SymbolTable *symbol_table, FILE *file) {
    // Run and expand all macros in the program.
    // Return number of macros
    int is_in_macro = 0;
    int macroCount = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char macroName[MAX_LABEL_LENGTH];
        if (!ignore_line(line)) {
            if (isMacroDefinitionStart(line)) {
                is_in_macro = 1;

                sscanf(line, "%*s %s", macroName);  // Skip "%macro" and capture the name

                if (macroCount < MAX_MACRO_NAMES) {
                    strcpy(macroNames[macroCount++], macroName);
                } else {
                    fprintf(stderr, "Warning: Reached maximum number of macro names (%d)\n", MAX_MACRO_NAMES);
                }
                is_in_macro = handleMacroDefinition(file, macroTable, line); // Pass macroTable directly
            } else if (isMacroInvocation(line, macroName, macroNames)) {
                Macro macro;
                expandMacro(&macro, stdout);
                writeExpandedMacrosToFile(macroTable);

            } else {
                if (!is_in_macro) {
                    write_line_to_file(line);
                }
            }
        }
    }
}

void read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro) {
    const char *directive;
    char label[MAX_LABEL_LENGTH] = {0};
    int hasLabel;

    line = skip_spaces(line);

    printf("Lineee: %s\n", line);


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
        }
    } else if (is_command(line, ic)) {
        printf("is_command");
        handleCommand(line, ic);
    }
}

int write_line_to_file(char *line) {
    FILE *outputFile = fopen(macroFileName, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n", macroFileName);
        return 1;  // Indicate failure
    }
    lower_string(line);
    // Write the string to the file
    fprintf(outputFile, "%s", line);

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

bool isMacroDefinitionStart(char *line) {
    return strstr(line, "macr ") != NULL;
}

bool isMacroDefinitionEnd(char *line) {
    return strstr(line, "endmacr") != NULL;
}

int isMacroInvocation(char *line, char *macroName, char **macroNames) {
    // Extract potential macro name
    sscanf(line, "%s", macroName);

    // Check if the extracted name is not empty and exists in macroNames
    if (macroName[0] != '\0') {
        for (int i = 0; i < MAX_MACRO_NAMES && macroNames[i][0] != '\0'; ++i) {
            if (strcmp(macroName, macroNames[i]) == 0) {
                return 1; // Macro name found in the array
            }
        }
    }

    return 0; // Not a macro invocation or name not found
}

// Return 0 if ended, else 1
int handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine) {
    Macro macro;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LABEL_LENGTH];

    sscanf(firstLine, "%s", macroName);
    strcpy(macro.name, macroName + 7);  // Skip "%macro"
    macro.lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isMacroDefinitionEnd(line)) {
            addMacro(macroTable, macro);
            return 0;
        }
        lower_string(line);
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

void handleCommand(char *line, int *ic) {
    char *line_content = line; // String containing the assembly instruction (content of the line)
    size_t length = strlen(line);  // Length of the line (excluding null terminator)
    int instruction_type = IS_COMMAND; // is it directive or command
    enum opcode_command opcode_command_type; // enum containing enum values for opcode command types if it's an opcode

    unsigned int operand_number_value; // int containing the operand number

    char *first_operand = NULL; // string containing the first operand r0-r7 (can be null)
    enum operand_classification_type first_operand_classification_type; // int containing enum values for first operand classification type
    char *second_operand = NULL; // string containing the second operand r0-r7 (can be null)
    enum operand_classification_type second_operand_classification_type; // int containing enum values for second_operand classification type

    enum directives directive_type; // enum containing enum values for directive type if it's a directive

    char command_name[MAX_COMMAND_LEN];
    // Extract instruction name
    sscanf(line, "%s", command_name);

    // Get the operand number
    opcode_command = operand_number(command_name);

    (*ic)++;
}


int operand_number(char *command_name){
    unsigned int operand_number;
    if (strcmp(command_name, "mov") == 0) {
        operand_number = MOV;
    } else if (strcmp(command_name, "cmp") == 0) {
        operand_number = CMP;
    } else if (strcmp(command_name, "add") == 0) {
        operand_number = ADD;
    } else if (strcmp(command_name, "sub") == 0) {
        operand_number = SUB;
    } else if (strcmp(command_name, "not") == 0) {
        operand_number = NOT;
    } else if (strcmp(command_name, "clr") == 0) {
        operand_number = CLR;
    } else if (strcmp(command_name, "lea") == 0) {
        operand_number = LEA;
    } else if (strcmp(command_name, "inc") == 0) {
        operand_number = INC;
    } else if (strcmp(command_name, "dec") == 0) {
        operand_number = DEC;
    } else if (strcmp(command_name, "jmp") == 0) {
        operand_number = JMP;
    } else if (strcmp(command_name, "bne") == 0) {
        operand_number = BNE;
    } else if (strcmp(command_name, "red") == 0) {
        operand_number = RED;
    } else if (strcmp(command_name, "prn") == 0) {
        operand_number = PRN;
    } else if (strcmp(command_name, "jsr") == 0) {
        operand_number = JSR;
    } else if (strcmp(command_name, "rts") == 0) {
        operand_number = RTS;
    } else if (strcmp(command_name, "stop") == 0) {
        operand_number = STOP;
    }

    return operand_number;
}

int ignore_line(char *line) {
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

char* skip_spaces(char *line) {
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

// Function to check if a line is a label
int find_label(char *line, char *label) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    char *start = line;

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
int is_command(char *line, int *ic) {
    lower_string(line);
    printf("start find command");
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

int symbolExists(SymbolTable *table, char *label) {
    for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->symbols[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a line is a directive
bool is_directive(char *line) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return *line == '.';
}

bool isDataDirective(char *line) {
    return strstr(line, ".data") != NULL;
}

bool isStringDirective(char *line) {
    return strstr(line, ".string") != NULL;
}

bool isExternDirective(char *line) {
    return strstr(line, ".extern") != NULL;
}

bool isEntryDirective(char *line) {
    return strstr(line, ".entry") != NULL;
}

void handleDataDirective(char *line, int *dc) {
    // Implementation here
}

void handleStringDirective(char *line, int *dc) {
    // Implementation here
}
