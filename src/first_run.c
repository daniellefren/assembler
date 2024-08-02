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

char macroFileName[] = "expanded_macros.am";

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

void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, LabelTable *label_table) {
    char line[MAX_LINE_LENGTH];
    char *macroNames[MAX_MACRO_NAMES];  // Array to store pointers to macro names
    int line_num = 1;
    int is_in_macro = 0;
    MacroTable macroTable;

    initMacroTable(&macroTable);
    initMacroNameArray(macroNames);

    // Reset file pointer to the beginning before calling pre_run
    rewind(file);
    pre_run(line, &macroTable, macroNames, file); // Keeps track of the number of encountered macros

    FILE *expanded_macros_file = fopen(macroFileName, "r");

    while (fgets(line, MAX_LINE_LENGTH, expanded_macros_file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, label_table, ic, dc, is_in_macro, lines_array);
        }
        line_num++;
    }

    fclose(file);

    // Free allocated memory for macro names
    for (int j = 0; j < MAX_MACRO_NAMES; ++j) {
        free(macroNames[j]);
    }
}

void pre_run(char *line, MacroTable *macroTable, char **macroNames, FILE *file) {
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

void read_line(char *line, LabelTable *label_table, int *ic, int *dc, int is_in_macro, LinesArray *lines_array) {
    char label[MAX_LABEL_LENGTH] = "";
    int hasLabel;

    line = skip_spaces(line);

    InstructionLine *new_instruction_line;

    printf("Get instruction line %s", new_instruction_line->line_content);
    Label *new_label;
    hasLabel = find_label(line, label);

    if (hasLabel) {
        // TODO - check if label is not a known word in assembly
        line = strchr(line, ':') + 1;
        // Skip spaces after .data directive
        while (*line && isspace((unsigned char)*line)) {
            line++;
        }
        new_label = (Label *)malloc(10 * sizeof(Label));
        if (new_label == NULL) {
            fprintf(stderr, "Memory allocation error for directive\n");
            exit(EXIT_FAILURE);
        }

        strcpy(new_label->name, label);
        new_instruction_line->is_label=1;
        new_instruction_line->label = new_label;

    }

    if (isDataDirective(line)) { // If .data or .String
        printf("starting isDataDirective");
        new_instruction_line->instruction_type=DATA_DIRECTIVE;
        if(hasLabel){
            new_label->type = DATA_DIRECTIVE;
        }

        Directive *new_directive = (Directive *)malloc(10 * sizeof(Directive));
        if (new_directive == NULL) {
            fprintf(stderr, "Memory allocation error for directive\n");
            exit(EXIT_FAILURE);
        }

        handleDirectives(line, dc, new_directive);

        new_instruction_line->directive = new_directive;

        printf("End isDataDirective");
    }
    else if (is_command(line, ic)) {
        printf("is command");
        new_instruction_line->instruction_type=COMMAND;

        if(hasLabel){
            new_label->type = COMMAND;
        }

        Command *new_command = (Command *)malloc(10 * sizeof(Command));
        if (new_command == NULL) {
            fprintf(stderr, "Memory allocation error for directive\n");
            exit(EXIT_FAILURE);
        }

        handleCommand(line, ic, new_command, label_table);

        new_instruction_line->command = new_command;

    }

    if(hasLabel){
        new_instruction_line->label = new_label;
        addNewLabel(label_table, new_label);
    }

    addInstructionLine(lines_array, new_instruction_line);

}

void init_instruction_line(InstructionLine *new_instruction_line, char* line){
    // Allocate memory for instruction line
    new_instruction_line = (InstructionLine *)malloc(sizeof(InstructionLine));
    if (new_instruction_line == NULL) {
        fprintf(stderr, "Memory allocation error for InstructionLine\n");
        exit(EXIT_FAILURE);
    }

    new_instruction_line->line_content = strdup(line);

    if (new_instruction_line->line_content == NULL) {
        fprintf(stderr, "Memory allocation error for line content\n");
        free(new_instruction_line);
        exit(EXIT_FAILURE);
    }

    new_instruction_line->length = strlen(line);
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

void handleCommand(char *line, int *ic, Command *new_command, LabelTable *label_table) {
    printf("handleCommand\n");

    new_command->src_operand = (Operand *)malloc(sizeof(Operand));
    new_command->dst_operand = (Operand *)malloc(sizeof(Operand));;

    unsigned int operands_number;
    int opcode_command_type;

    // Allocate memory for operands
    *(new_command->src_operand->value) = (char *)malloc(MAX_OPERAND_SIZE * sizeof(char));
    *(new_command->dst_operand->value) = (char *)malloc(MAX_OPERAND_SIZE * sizeof(char));

    if (!(new_command->src_operand->value) || !(new_command->dst_operand->value)) {
        fprintf(stderr, "Memory allocation error for operands\n");
        free(new_command->src_operand->value);
        free(new_command->dst_operand->value);
        exit(EXIT_FAILURE);
    }

    sscanf(line, "%s", new_command->command_name);

    getCommandData(new_command->command_name, new_command);

    defineOperandsFromLine(new_command, line);
    defineOperandTypes(new_command->src_operand, label_table);
    defineOperandTypes(new_command->dst_operand, label_table);


    classify_operand(new_command->src_operand);
    classify_operand(new_command->dst_operand);

    //TODO - find the number that the ic should ascend
    (*ic)++;
}

int checkIfOperandLabel(char *operand, LabelTable *label_table) {
    // Check if operand is a label and return the index in the labels array
    for (int i = 0; i < label_table->size; i++) {
        if (strcmp(operand, label_table->labels[i].name) == 0) {
            return i;
        }
    }
    return -1;
}


void defineOperandsFromLine(Command *new_command, char* line){
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

    switch (new_command->operand_number) {
        case 0:
            return;
        case 1:
            // Extract the first operand
            sscanf(line, "%s", new_command->src_operand->value);
            return;

        case 2:
            // Extract the first operand
            sscanf(line, "%[^,]", new_command->src_operand->value);

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
            sscanf(line, "%s", new_command->dst_operand->value);
            return;

        default:
            fprintf(stderr, "Invalid number of operands: %d\n", new_command->operand_number);
            exit(EXIT_FAILURE);
    }
}

void defineOperandTypes(Operand *operand, LabelTable *label_table){
    operand->type = INVALID;
    int is_valid = 1;
    if (new_operand->value[0] == '#') {
        // Check if the rest is a valid integer
        for (int i = 1; new_operand->value[i] != '\0'; ++i) {
            if (!isdigit(new_operand->value[i]) && new_operand->value[i] != '-') {
                is_valid = 0;
            }
        }
        if(is_valid){
            operand->type = INTEGER;
        }
    }
    else if (new_operand->value[0] == '*') {
        if (new_operand->value[1] == 'r' && isdigit(new_operand->value[2])){
                operand->type = REGISTER;
        }
    }

    else if (new_operand->value[0] == 'r' && isdigit(new_operand->value[1])){
            operand->type = REGISTER;
    }

    else if(labelExists(label_table, new_operand->value)){
        operand->type = LABEL
    }
}

//classify the operand addressing mode
void classify_operand(Operand *new_operand) {
    new_operand->classification_type = METHOD_UNKNOWN; // set default

    // Immediate addressing - starts with #
    if (new_operand->type == INTEGER) {
        new_operand->classification_type = IMMEDIATE; // Immediate addressing
    }
    else if(new_operand->type == LABEL){
        //TODO - what if the label is defined after
        new_operand->classification_type = DIRECT; // Direct addressing
    }

    // Register indirect addressing - starts with *
    else if (new_operand->value[0] == '*' && new_operand->type == REGISTER) {
            new_operand->classification_type = INDIRECT_REGISTER; // Indirect Register addressing
    }

    // Register addressing - starts with r followed by a digit
    else if (new_operand->type == REGISTER) {
        new_operand->classification_type = DIRECT_REGISTER; // Direct Register addressing
    }

}

// Get the number of operands should be in the given command
void getCommandData(char* command_name, Command *new_command){
    printf("getCommandData");
    int num_commands = sizeof(commands_struct) / sizeof(commands_struct[0]); // Calculate number of instructions
    int command_opcode;
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(commands_struct[i].command_name, command_name) == 0) {
            new_command->opcode_command_type = commands_struct[i].opcode_command_type;
            new_command->operand_number=commands_struct[i].operand_number;
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
    for (int i = 0; i < sizeof(commands); i++) {

        if (strncmp(line, commands[i], strlen(commands[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

int labelExists(LabelTable *label_table, char *label) {
    for (size_t i = 0; i < label_table->size; ++i) {
        if (strcmp(label_table->labels[i].name, label) == 0) {
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

void handleDirectives(char *line, int *dc, Directive *new_directive) {
    //TODO - handle .extern

    char directive_type[MAX_LINE_LENGTH];
    char *ptr = line;
    char *values[MAX_LINE_LENGTH]; // Temporary array to hold values as strings
    size_t values_count = 0;

    sscanf(line, "%s", directive_type);

    if (strcmp(directive_type, ".data") == 0) {
        printf(".data\n");
        new_directive->type = DATA;
        ptr += strlen(directive_type);

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
    } else if (strcmp(directive_type, ".string") == 0) {
        new_directive->type = STRING;
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
        new_directive->type = NOT_DIRECTIVE;
        fprintf(stderr, "Error: Unknown directive %s\n", directive_type);
    }

    // Allocate memory for the values in the symbol
    new_directive->value = (char **)malloc(values_count * sizeof(char *));

    if (new_directive->value == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    // Copy the values to the symbol
    for (size_t i = 0; i < values_count; i++) {
        new_directive->value[i] = values[i];
    }
    new_directive->data_values_count = values_count;


    debuggingData(new_directive);
}


void debuggingData(Directive *new_directive){
    //TODO - erase this function
    printf("???\n");
    // Print for debugging
    printf("Type: %d\n", new_directive->type);
    printf("Values count: %zu\n", new_directive->data_values_count);
    printf("Values: ");
    for (size_t j = 0; j < new_directive->data_values_count; j++) {
        printf("%s ", ((char **)new_directive->value)[j]);
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

