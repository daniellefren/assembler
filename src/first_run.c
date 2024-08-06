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

//TODO - get all errors from code and not exit after one

Command commands_struct[] = {
        {"mov", MOV, 2},
        {"cmp", CMP, 2},
        {"add", ADD, 2},
        {"sub", SUB, 2},
        {"lea", LEA, 2},
        {"not", NOT, 1},
        {"clr", CLR, 1},
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
    MacroTable macroTable;

    init_macro_table(&macroTable);
    init_macro_name_array(macroNames);

    // Reset file pointer to the beginning before calling pre_run
    rewind(file);

    pre_run(line, &macroTable, macroNames, file); // Keeps track of the number of encountered macros


    FILE *expanded_macros_file = fopen(macroFileName, "r");


    while (fgets(line, MAX_LINE_LENGTH, expanded_macros_file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, label_table, ic, dc, lines_array);
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
    erase_file_data(macroFileName);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char macroName[MAX_LABEL_LENGTH];
        if (!ignore_line(line)) {
            if (is_macro_definition_start(line)) {
                is_in_macro = 1;

                sscanf(line, "%*s %s", macroName);  // Skip "%macro" and capture the name
                if(is_known_assembly_keyword(macroName)){
                    fprintf(stderr, "Macro %s is known assembly keyword\n", macroName);
                    exit(EXIT_FAILURE);
                }

                if (macroCount < MAX_MACRO_NAMES) {
                    strcpy(macroNames[macroCount++], macroName);
                } else {
                    fprintf(stderr, "Warning: Reached maximum number of macro names (%d)\n", MAX_MACRO_NAMES);
                }
                is_in_macro = handle_macro_definition(file, macroTable, line); // Pass macroTable directly
            } else if (is_macro_invocation(line, macroName, macroNames)) {
                Macro macro;
                expand_macro(&macro, stdout);
                write_expanded_macros_to_file(macroTable);

            } else {
                if (!is_in_macro) {
                    write_line_to_file(line);
                }
            }
        }
    }
}

void read_line(char *line, LabelTable *label_table, int *ic, int *dc, LinesArray *lines_array) {
    char label[MAX_LABEL_LENGTH] = "";
    int hasLabel;

    line = skip_spaces(line);
    printf("line %s\n", line);

    InstructionLine *new_instruction_line = init_instruction_line(line);

    Label *new_label;
    hasLabel = find_label(line, label);

    if (hasLabel) {
        line = strchr(line, ':') + 1;
//         Skip spaces after .data directive
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

        new_label->address = ic;
    }

    if (is_data_directive(line)) { // If .data or .String
        new_instruction_line->instruction_type=DATA_DIRECTIVE;
        if(hasLabel){
            new_label->type = DATA_DIRECTIVE;
        }

        Directive *new_directive = init_directive();

        handle_directives(line, dc, new_directive);

        new_instruction_line->directive = new_directive;

    }
    else if (is_command(line, ic)) {
        new_instruction_line->instruction_type=COMMAND;
        if(hasLabel){
            new_label->type = COMMAND;
        }
        Command *new_command = init_command();
        handle_command(line, new_command, label_table);
        new_instruction_line->binary_line_count = find_number_of_lines_in_binary(new_command);

        (*ic)+= new_instruction_line->binary_line_count;

        new_instruction_line->command = new_command;
    }
    if(hasLabel){
        new_instruction_line->label = new_label;
        addNewLabel(label_table, new_label);
    }
    addInstructionLine(lines_array, new_instruction_line);

}


int write_line_to_file(char *line) {
    FILE *outputFile = fopen(macroFileName, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n", macroFileName);
        exit(EXIT_FAILURE);
    }
//    lower_string(line);
    // Write the string to the file
    fprintf(outputFile, "%s", line);

    fclose(outputFile);
    return 0;  // Indicate success
}

void write_expanded_macros_to_file(MacroTable *macroTable) {
    FILE *outputFile = fopen(macroFileName, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing macros\n", macroFileName);
        exit(EXIT_FAILURE);
    }

    // Write each macro's body to the file
    for (int i = 0; i < macroTable->count; ++i) {
        for (int j = 0; j < macroTable->macros[i].lineCount; ++j) {
            fprintf(outputFile, "%s", macroTable->macros[i].body[j]);
            exit(EXIT_FAILURE);
        }
    }

    fclose(outputFile);
}

void add_macro(MacroTable *table, Macro macro) {
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

bool is_macro_definition_start(char *line) {
    return strstr(line, "macr ") != NULL;
}

bool is_macro_definition_end(char *line) {
    return strstr(line, "endmacr") != NULL;
}

int is_macro_invocation(char *line, char *macroName, char **macroNames) {
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
int handle_macro_definition(FILE *file, MacroTable *macroTable, const char *firstLine) {
    Macro macro;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LABEL_LENGTH];

    sscanf(firstLine, "%s", macroName);
    strcpy(macro.name, macroName + 7);  // Skip "%macro"
    macro.lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (is_macro_definition_end(line)) {
            add_macro(macroTable, macro);
            return 0;
        }
        lower_string(line);
        strcpy(macro.body[macro.lineCount++], line);
    }

    add_macro(macroTable, macro);
    return 1;
}

void expand_macro(const Macro *macro, FILE *outputFile) {
    for (int i = 0; i < macro->lineCount; ++i) {
        fprintf(outputFile, "%s", macro->body[i]);
    }
}

//Handle when assembly line is a command
void handle_command(char *line, Command *new_command, LabelTable *label_table) {
    sscanf(line, "%s", new_command->command_name);

    get_command_data(new_command->command_name, new_command);

    define_operands_from_line(new_command, line);
    switch (new_command->operand_number) {
        case 1:
            define_operand_types(new_command->src_operand, label_table);
            classify_operand(new_command->src_operand);
            break;
        case 2:
            define_operand_types(new_command->src_operand, label_table);
            define_operand_types(new_command->dst_operand, label_table);

            classify_operand(new_command->src_operand);
            classify_operand(new_command->dst_operand);
            break;
    }
}


void define_operands_from_line(Command *new_command, char* line){
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

            // Extract the second operand
            extract_second_operand_from_line(line, new_command);
            return;

        default:
            fprintf(stderr, "Invalid number of operands: %d\n", new_command->operand_number);
            exit(EXIT_FAILURE);
    }
}

void extract_second_operand_from_line(char* line, Command *new_command){
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
}

void define_operand_types(Operand *operand, LabelTable *label_table){
    operand->type = INVALID;
    int is_valid = 1;
    if (operand->value[0] == '#') {
        int length = 0;
        // Check if the rest is a valid integer
        for (int i = 1; operand->value[i] != '\0'; ++i) {
            if (!isdigit(operand->value[i]) && operand->value[i] != '-') {
                is_valid = 0;
                ++length;
            }
        }

        if(is_valid){
            operand->type = INTEGER;
            strcpy(operand->value, extract_numbers(operand->value, length));
        }
    }
    else if (operand->value[0] == '*') {
        if (operand->value[1] == 'r' && operand->value[2] >= '1' && operand->value[2] <= '7'){
            operand->type = REGISTER;
        }
    }

    else if (operand->value[0] == 'r' && operand->value[1] >= '1' && operand->value[1] <= '7'){
            operand->type = REGISTER;
    }

    else if(is_valid_label(operand->value)){
        operand->type = LABEL;
    }
    else{
        fprintf(stderr, "Not valid operand type. %s\n", operand->value);
        exit(EXIT_FAILURE);
    }
}

// Function to check if a string is a valid label
int is_valid_label(const char *label) {
    //TODO - add exit when not valid
    //Check length of the label
    size_t length = strlen(label);
    if (length == 0 || length > MAX_LABEL_LENGTH) {
        return 0;
    }

    //check if not assembly keyword
    if(is_known_assembly_keyword(label)){
        return 0;
    }

    //TODO -  check if Label matches an existing macro name
//    for (int i = 0; i < macroTable->count; i++) {
//        if (strcmp(label, macroTable->macros[i].name) == 0) {
//            return 0; // Label matches an existing macro name
//        }
//    }

    //Check if the first character is a letter
    if (!isalpha((unsigned char)label[0])) {
        return 0;
    }

    //  Check if other characters are letters/numbers
    for (size_t i = 1; i < length; ++i) {
        if (!isalnum((unsigned char)label[i])) {
            return 0;
        }
    }

    return 1; // Valid label
}

int find_number_of_lines_in_binary(Command *new_command){
    int number_of_binary_lines = new_command->operand_number; //The default for number of binary lines for data word is the operand number

    //If both operands are indirective or directive register classification, then they share the same data word
    if(new_command->operand_number == 2){
        if((new_command->src_operand->classification_type==INDIRECT_REGISTER && new_command->dst_operand->classification_type==INDIRECT_REGISTER) ||
                (new_command->src_operand->classification_type==DIRECT_REGISTER && new_command->dst_operand->classification_type==DIRECT_REGISTER)){
            number_of_binary_lines -= 1;
        }
    }

    return number_of_binary_lines;
}

//classify the operand addressing mode
void classify_operand(Operand *new_operand) {
    new_operand->classification_type = METHOD_UNKNOWN; // set default

    // Immediate addressing - starts with #
    if (new_operand->type == INTEGER) {
        new_operand->classification_type = IMMEDIATE; // Immediate addressing
    }
    else if(new_operand->type == LABEL){
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
    else{
        fprintf(stderr, "Not valid classification type");
        exit(EXIT_FAILURE);
    }

}

// Get the number of operands should be in the given command
void get_command_data(char* command_name, Command *new_command){
    int command_opcode;
    for (int i = 0; i < COMMANDS_COUNT; i++) {
        if (strcmp(commands_struct[i].command_name, command_name) == 0) {
            new_command->opcode_command_type = commands_struct[i].opcode_command_type;
            new_command->operand_number=commands_struct[i].operand_number;
            return;
        }
    }
    // Handle the case where the opcode is not found
    fprintf(stderr, "Error: Opcode %d not found in the instruction set.\n", command_opcode);
    exit(EXIT_FAILURE);
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
    // Skip leading whitespace
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    char *start = line;

    // Find the end of the label (until space or colon)
    while (*start && !isspace((unsigned char)*start) && *start != ':') {
        start++;
    }
    // If a colon is found, we assume it is a label
    if (*start == ':') {
        size_t len = start - line;
        strncpy(label, line, len);
        label[len] = '\0'; // Null-terminate the label

        // TODO - maybe check here if the label valid
        return 1;
    }
    if(((*start == ' ') && *(start + 1) == ':')){
        fprintf(stderr, "Label is not valid -- a space between the label and the :\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

// Return 1 if label is an assembly keyword, else 0
int is_known_assembly_keyword(const char *label) {
    //TODO - Know if label or macro
    // Copy commands into all_instructions
    for (size_t i = 0; i < COMMANDS_COUNT; ++i) {
        if (strcmp(label, commands[i]) == 0) {
            fprintf(stderr, "Label is not valid -- A known assembly keyword\n");
            return 1; //Label is a known command
        }
    }

    // Copy directives into all_instructions
    for (size_t j = 0; j < DIRECTIVES_COUNT; ++j) {
        if (strcmp(label, directives[j]) == 0) {
            fprintf(stderr, "Label is not valid -- A known assembly keyword\n");
            return 1; //Label is a known directive
        }
    }

    return 0;
}

// Function to check if a line is an instruction
int is_command(char *line, int *ic) {
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (int i = 0; i < COMMANDS_COUNT; i++) {
        if (strncmp(line, commands[i], strlen(commands[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

int label_exists(LabelTable *label_table, char *label) {
    for (size_t i = 0; i < label_table->size; ++i) {
        if (strcmp(label_table->labels[i].name, label) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a line is a data or string directive
bool is_data_directive(char *line) {
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

void handle_directives(char *line, int *dc, Directive *new_directive) {
    //TODO - handle .extern
    //TODO - return value 1 when error
    //TODO - split to functions

    char directive_type[MAX_LINE_LENGTH];
    char *ptr = line;
    char *values[MAX_LINE_LENGTH]; // Temporary array to hold values as strings
    size_t values_count = 0;

    // Read the directive type from the line
    if (sscanf(line, "%s", directive_type) != 1) {
        fprintf(stderr, "Error: Failed to read directive type\n");
        new_directive->type = NOT_DIRECTIVE;
        exit(EXIT_FAILURE);
    }

    if (strcmp(directive_type, ".data") == 0) {
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
        exit(EXIT_FAILURE);
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
//    debugging_data(new_directive);

}


void debugging_data(Directive *new_directive){
    //TODO - erase this function
    printf("enter debugging\n");
    // Print for debugging
    printf("Type: %d\n", new_directive->type);
    printf("Values count: %zu\n", new_directive->data_values_count);
    printf("Values: ");
    for (size_t j = 0; j < new_directive->data_values_count; j++) {
        printf("%s ", ((char **)new_directive->value)[j]);
    }
    printf("\n");
    printf("finish debuggingData\n");
}


bool is_extern_directive(char *line) {
    return strstr(line, ".extern") != NULL;
}

bool is_entry_directive(char *line) {
    return strstr(line, ".entry") != NULL;
}

