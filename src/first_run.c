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


void initMacroTable(MacroTable *table) {
    table->macros = (Macro *)malloc(10 * sizeof(Macro));
    table->count = 0;
    table->capacity = 10;
}

void addMacro(MacroTable *table, Macro macro) {
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->macros = (Macro *)realloc(table->macros, table->capacity * sizeof(Macro));
    }
    table->macros[table->count++] = macro;
}

bool findMacro(const MacroTable *table, const char *name, Macro *macro) {
    for (int i = 0; i < table->count; ++i) {
        if (strcmp(table->macros[i].name, name) == 0) {
            *macro = table->macros[i];
            return true;
        }
    }
    return false;
}

bool isMacroDefinitionStart(const char *line) {
//    printf("is macro start %s", line);
    return strstr(line, "macr") != NULL;
}

bool isMacroDefinitionEnd(const char *line) {
//    printf("is macro ending %s", line);

    return strstr(line, "endmacr") != NULL;
}

bool isMacroInvocation(const char *line, char *macroName) {
    printf("check if is macro invoke %s\n", line);


    printf("isMacroInvocation");
    sscanf(line, "%s", macroName);
    return macroName[0] != '\0';
}

void handleMacroDefinition(FILE *file, MacroTable *macroTable, const char *firstLine) {
    printf("handleMacroDefinition");
    Macro macro;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LABEL_LENGTH];

    sscanf(firstLine, "%s", macroName);
    strcpy(macro.name, macroName + 7);  // Skip "%macro"
    macro.lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isMacroDefinitionEnd(line)) {
            printf("\nMacro ended successfully\n");
            break;
        }
        strcpy(macro.body[macro.lineCount++], line);
    }

    addMacro(macroTable, macro);
}

void expandMacro(const Macro *macro, FILE *outputFile) {
    for (int i = 0; i < macro->lineCount; ++i) {
        fprintf(outputFile, "%s", macro->body[i]);
    }
}


void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table){
    char line[MAX_LINE_LENGTH];
    int line_num = 1;
    MacroTable macroTable;
    initMacroTable(&macroTable);

    char macroNames[MAX_MACRO_NAMES][MAX_LABEL_LENGTH];  // Array to store macro names
    int macroCount = 0;  // Keeps track of the number of encountered macros


    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, &macroTable, symbol_table, ic, dc);
        }

        if (isMacroDefinitionStart(line)) {
            // Extract macro name on encountering a macro definition
            char macroName[MAX_LABEL_LENGTH];
            sscanf(line, "%*s %s", macroName);  // Skip "%macro" and capture the name

            if (macroCount < MAX_MACRO_NAMES) {
                strcpy(macroNames[macroCount++], macroName);
            } else {
                fprintf(stderr, "Warning: Reached maximum number of macro names (%d)\n", MAX_MACRO_NAMES);
            }
            handleMacroDefinition(file, &macroTable, line);
        }

        line_num++;
    }

    printf("\nFound %d macros:\n", macroCount);
    for (int i = 0; i < macroCount; ++i) {
        printf("%s\n", macroNames[i]);
    }
}

void read_line(const char *line, MacroTable *macroTable, SymbolTable *symbol_table, int *ic, int *dc) {
    const char *directive;
    char label[MAX_LABEL_LENGTH] = {0};
    bool hasLabel;
    char macroName[MAX_LABEL_LENGTH];

//    printf("%s\n", line);

    line = skip_spaces(line);

    hasLabel = find_label(line, label);
//    printf("label %s\n", label);

    if(hasLabel) {
        line = strchr(line, ':') + 1;
    }

    if(isMacroInvocation(line, macroName)){
//        printf("yesyes");
        Macro *macro;
        if(findMacro(macroTable, macroName, macro)){
            expandMacro(macro, stdout);
        } else {
            fprintf(stderr, "Error: Macro '%s' not defined\n", macroName);
        }
        return;
    }
    if(is_directive(line)){
        if(hasLabel){
            addSymbol(symbol_table, label, *dc, directive);
        }
        if(isDataDirective(line)){
            printf("data directive %s\n", directive);
            handleDataDirective(line, dc);
        } else if(isStringDirective(line)) {
            printf("String directive %s\n", directive);
            handleStringDirective(line, dc);
        }
//          else if(isEntryDirective(line)) {
//            printf("entry directive %s\n", directive);
//        } else if(isExternDirective(line)){
//            printf("extern directive %s\n", directive);
//        }  else{
//            printf("struct directive %s\n", directive);
//        }

        else{
            if(is_instruction(line, ic)){
                handleInstruction(line, ic);
            }
        }

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
    // This function should parse the instruction and update the IC accordingly
    // For simplicity, let's assume each instruction takes one memory word
    printf("operand\n");
    (*ic)++;
}

int ignore_line(const char *line){
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

const char* skip_spaces(const char *line){
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

// Function to check if a line is a label
bool find_label(const char *line, char *label) {
    // Function to check if a line is a label and return the label
    // Skip leading whitespace
    while (*line && isspace((unsigned char) *line)) {
        line++;
    }

    // Start of the potential label
    const char *start = line;

    // Find the end of the label (colon or whitespace)
    while (*line && !isspace((unsigned char) *line) && *line != ':') {
        line++;
    }

    // Check if the character after the label is a colon
    if (*line == ':') {
        size_t len = line - start;
        strncpy(label, start, len);
        label[len] = '\0'; // Null-terminate the label
        return true;
    }

    return false;
}

// Function to check if a line is an operand (simple check)
bool is_instruction(const char *line, int *ic) {
    // An operand line typically starts with an instruction or mnemonic
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (strncmp(line, commands[i], strlen(commands[i])) == 0) {
            return true;
        }
    }
    return false;
}

bool symbolExists(const SymbolTable *table, const char *label) {
    for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->symbols[i].label, label) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a line is a directive
bool is_directive(const char *line) {
    // A directive typically starts with a period
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

void handleDataDirective(const char *line, int *dc){

}
void handleStringDirective(const char *line, int *dc){

}

