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

Command commands_struct[] = {
        {"mov", MOV, 2},
        {"cmp", CMP, 2},
        {"add", ADD, 2},
        {"sub", SUB, 2},
        {"not", NOT, 1},
        {"clr", CLR, 1},
        {"lea", LEA, 2},
        {"inc", INC, 1},
        {"dec", DEC, 1},
        {"jmp", JMP, 1},
        {"bne", BNE, 1},
        {"red", RED, 1},
        {"prn", PRN, 1},
        {"jsr", JSR, 1},
        {"rts", RTS, 0},
        {"stop", STOP, 0},
        {"not_opcode", NOT_OPCODE, 0}
};

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
            read_line(line, symbol_table, ic, dc, is_in_macro, lines_array);
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

void read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int is_in_macro, LinesArray *lines_array) {
    const char *directive;
    char label[MAX_LABEL_LENGTH] = "";
    int hasLabel;

    line = skip_spaces(line);

    printf("Line: %s", line);

    hasLabel = find_label(line, label);
    printf("here!");
    if (hasLabel) {
        // TODO - check if label is not a known word in assembly
        line = strchr(line, ':') + 1;
        // Skip spaces after .data directive
        while (*line && isspace((unsigned char)*line)) {
            line++;
        }
    }
    printf("here?");

    if (isDataDirective(line)) {
        printf("isDataDirective");
        Symbol *new_symbol = (Symbol *)malloc(10 * sizeof(Symbol));
        if (new_symbol == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_symbol->label, label);
        handleDirectives(line, dc, symbol_table, new_symbol);

    }
    else if (is_command(line, ic)) {
        printf("is command");
        handleCommand(line, ic, lines_array, symbol_table);
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
    // TODO - check if macro is not a known word in assembly
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

void handleCommand(char *line, int *ic, LinesArray *lines_array, SymbolTable *symbol_table) {
    printf("handleCommand\n");

    // Allocate memory for instruction line
    InstructionLine *instruction_line = (InstructionLine *)malloc(sizeof(InstructionLine));
    if (instruction_line == NULL) {
        fprintf(stderr, "Memory allocation error for InstructionLine\n");
        exit(EXIT_FAILURE);
    }

    // Duplicate the line content
    instruction_line->line_content = strdup(line);
    if (instruction_line->line_content == NULL) {
        fprintf(stderr, "Memory allocation error for line content\n");
        free(instruction_line);
        exit(EXIT_FAILURE);
    }
    printf("\naaa\n");
    instruction_line->length = strlen(line);
    instruction_line->instruction_type = IS_COMMAND;

    // Initialize other members
    instruction_line->first_operand = NULL;
    instruction_line->second_operand = NULL;
    instruction_line->data_values = NULL;
    instruction_line->data_values_count = 0;

    unsigned int operands_number;
    int opcode_command_type;

    // Allocate memory for operands
    char *src_operand = (char *)malloc(MAX_OPERAND_SIZE * sizeof(char));
    char *dst_operand = (char *)malloc(MAX_OPERAND_SIZE * sizeof(char));
    if (!src_operand || !dst_operand) {
        fprintf(stderr, "Memory allocation error for operands\n");
        free(instruction_line->line_content);
        free(instruction_line);
        free(src_operand);
        free(dst_operand);
        exit(EXIT_FAILURE);
    }
    int src_operand_classification_type;
    int dst_operand_classification_type;

    char command_name[MAX_COMMAND_LEN];
    sscanf(line, "%s", command_name);
    // Ensure getOperandData and defineOperandsFromLine handle their parameters correctly
    getOperandData(command_name, instruction_line);

    defineOperandsFromLine(instruction_line->operand_number, src_operand, dst_operand, line);


    // Check if operands are labels and handle them accordingly
    int label_index_a = checkIfOperandLabel(src_operand, symbol_table);
    if (label_index_a != -1) {
        printf("src operand is label!\n");
        // Handle label properly - assume some appropriate handling, e.g., converting to a string representation
        snprintf(src_operand, MAX_OPERAND_SIZE, "%d", label_index_a);
    }

    int label_index_b = checkIfOperandLabel(dst_operand, symbol_table);
    if (label_index_b != -1) {
        printf("dst operand is label!\n");
        // Handle label properly - assume some appropriate handling, e.g., converting to a string representation
        snprintf(dst_operand, MAX_OPERAND_SIZE, "%d", label_index_b);
    }

    classify_operand(src_operand, &src_operand_classification_type);
    classify_operand(dst_operand, &dst_operand_classification_type);

    // Add instruction line to lines array
    addInstructionLine(lines_array, instruction_line);

    // Free allocated memory
    free(src_operand);
    free(dst_operand);
    free(instruction_line->line_content); // Ensure this is freed after it's no longer needed
    free(instruction_line);

    (*ic)++;
}

int checkIfOperandLabel(char *operand, SymbolTable *symbol_table) {
    // Check if operand is a label and return the index in the labels array
    for (int i = 0; i < symbol_table->size; i++) {
        if (strcmp(operand, symbol_table->symbols[i].label) == 0) {
            return i;
        }
    }
    return -1;
}


void defineOperandsFromLine(int operand_number_value, char *src_operand, char *dst_operand, char* line){
    // Skip leading spaces
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    // Find the command part and move past it
    while (*line != ' ' && *line != '\t' && *line != '\0') {
        line++;
    }

    // Skip spaces after command
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    switch (operand_number_value) {
        case 0:
            return;
        case 1:
            // Extract the first operand
            sscanf(line, "%s", src_operand);
            return;

        case 2:
            // Extract the first operand
            sscanf(line, "%[^,]", src_operand);

            // Move past the first operand and the comma
            line = strchr(line, ',');
            if (line != NULL) {
                line++; // Skip the comma
            }

            // Skip spaces before second operand
            while (*line == ' ' || *line == '\t') {
                line++;
            }

            // Extract the second operand
            sscanf(line, "%s", dst_operand);
            return;

        default:
            fprintf(stderr, "Invalid number of operands: %d\n", operand_number_value);
            exit(EXIT_FAILURE);
    }
}

//classify the operand addressing mode
void classify_operand(const char *operand, int *operand_type) {
    *operand_type = METHOD_UNKNOWN; // set default

    // Immediate addressing - starts with #
    if (operand[0] == '#') {
        // Check if the rest is a valid integer
        for (int i = 1; operand[i] != '\0'; ++i) {
            if (!isdigit(operand[i]) && operand[i] != '-') {
                *operand_type = METHOD_UNKNOWN; // Invalid operand
            }
        }

        *operand_type = IMMEDIATE; // Immediate addressing
    }

    // Register indirect addressing - starts with *
    if (operand[0] == '*') {
        if (operand[1] == 'r' && isdigit(operand[2])) {
            *operand_type = INDIRECT_REGISTER; // Register indirect addressing
        }
        *operand_type = METHOD_UNKNOWN; // Invalid operand
    }

    // Register addressing - starts with r followed by a digit
    if (operand[0] == 'r' && isdigit(operand[1])) {
        *operand_type = DIRECT_REGISTER; // Register addressing
    }

    // Direct addressing - assume any other valid identifier is a label
    // TODO
//    *operand_type = DIRECT; // Direct addressing


//    *operand_type = METHOD_UNKNOWN; // Invalid operand
}


int get_operand_opcode(char *command_name){
    //TODO - take from command_struct
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

// Get the number of operands should be in the given command
void getOperandData(char* command_name, InstructionLine *instruction_line){
    printf("getOperandData");
    int num_commands = sizeof(commands_struct) / sizeof(commands_struct[0]); // Calculate number of instructions
    int command_opcode;
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(commands_struct[i].command_name, command_name) == 0) {
            instruction_line->opcode_command_type=commands_struct[i].opcode;
            instruction_line->operand_number=commands_struct[i].num_of_operands;
            return;
        }
    }
    // Handle the case where the opcode is not found
    printf("Error: Opcode %d not found in the instruction set.\n", command_opcode);
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
    printf("find_label");
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    char *start = line;

    while (*start && !isspace((unsigned char)*start) && *start != ':') {
        start++;
    }

    if (*start == ':') {
        size_t len = start - line;
        strncpy(label, line, len);
        label[len] = '\0'; // Null-terminate the label
        line += len;

        return 1;
    }
    return 0;
}

// Function to check if a line is an instruction
int is_command(char *line, int *ic) {
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

// Function to check if a line is a data or string directive
bool isDataDirective(char *line) {
    // Skip leading spaces
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    // Check if the line starts with a dot and followed by "data" or "string"
    if (*line == '.') {
        line++;
        if (strncmp(line, "data", 4) == 0 && isspace((unsigned char)line[4])) {
            return true;
        }
        if (strncmp(line, "string", 6) == 0 && isspace((unsigned char)line[6])) {
            return true;
        }
    }
    return false;
}

void handleDirectives(char *line, int *dc, SymbolTable *symbol_table, Symbol *new_symbol) {
    //TODO - handle .extern

    char directive[MAX_LINE_LENGTH];
    char *ptr = line;
    char *values[MAX_LINE_LENGTH]; // Temporary array to hold values as strings
    size_t values_count = 0;

    sscanf(line, "%s", directive);

    if (strcmp(directive, ".data") == 0) {
        printf(".data\n");
        new_symbol->type = DATA;
        ptr += strlen(directive);

        while (*ptr != '\0') {
            while (*ptr == ' ' || *ptr == '\t' || *ptr == ',') {
                ptr++;
            }
            if (isdigit(*ptr) || (*ptr == '-' && isdigit(*(ptr + 1))) || (*ptr == '+' && isdigit(*(ptr + 1)))) {
                char *end;
                int number = strtol(ptr, &end, 10);
                ptr = end; // Move ptr to the end of the parsed number

                // Convert number to string and store in values
                char buffer[12]; // Buffer to hold the string representation of the number
                snprintf(buffer, sizeof(buffer), "%d", number);

                values[values_count] = strdup(buffer);
                values_count++;

                printf("!!! %s\n", values[values_count - 1]);
                (*dc)++;
            } else {
                ptr++;
            }
        }
    } else if (strcmp(directive, ".string") == 0) {
        new_symbol->type = STRING;
        // Find the first quote
        char *start = strchr(line, '\"');
        if (start) {
            // Find the closing quote
            char *end = strchr(start + 1, '\"');
            if (end) {
                // Copy the string contents excluding quotes
                for (char *p = start + 1; p < end; p++) {
                    char buffer[2] = {*p, '\0'};
                    values[values_count] = strdup(buffer);
                    values_count++;
                    (*dc)++;
                }
                // Add the null terminator to the end of the string
                values[values_count++] = strdup("\0");
                (*dc)++;
            }
        }
    } else {
        new_symbol->type = NOT_DIRECTIVE;
        fprintf(stderr, "Error: Unknown directive %s\n", directive);
    }

    // Allocate memory for the values in the symbol
    new_symbol->value = (char **)malloc(values_count * sizeof(char *));
    if (new_symbol->value == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    // Copy the values to the symbol
    for (size_t i = 0; i < values_count; i++) {
        new_symbol->value[i] = values[i];
    }
    new_symbol->data_values_count = values_count;

    addNewSymbol(symbol_table, new_symbol);

    debuggingData(new_symbol);
}

void handleDataDirective(){
    return;
}

void debuggingData(Symbol *new_symbol){
    //TODO - erase this function
    printf("???\n");
    // Print for debugging
    printf("Label: %s\n", new_symbol->label);
    printf("Type: %d\n", new_symbol->type);
    printf("Values count: %zu\n", new_symbol->data_values_count);
    printf("Values: ");
    for (size_t j = 0; j < new_symbol->data_values_count; j++) {
        printf("%s ", ((char **)new_symbol->value)[j]);
    }
    printf("\n");
    printf("???\n");
}


bool isExternDirective(char *line) {
    return strstr(line, ".extern") != NULL;
}

bool isEntryDirective(char *line) {
    return strstr(line, ".entry") != NULL;
}


void handleStringDirective() {
    // Implementation here
}

