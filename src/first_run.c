#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/first_run.h"
#include "../include/assembler.h"
#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/files_handler.h"


//TODO - get all errors from code and not exit after one and return also the line number

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

/**
 * Performs the first pass over the assembly source file.
 * The function processes the assembly source file to handle macro expansion, label handling, Commands and directive processing in the provided assembly file
 * This function also updates the instruction counter (IC) and data counter (DC) based on the processed lines.
 * @param file The input assembly source file to be processed.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param lines_array A pointer to the LinesArray structure that stores all lines of the source file after processing.
 * @param label_table A pointer to the LabelTable structure that stores all labels encountered in the source file.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 */
void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, LabelTable *label_table, int file_number) {
    char line[MAX_LINE_LENGTH];
    char *macroNames[MAX_MACRO_NAMES];  // Array to store pointers to macro names
    char expended_macro_file_name[100];
    MacroTable macro_table;
    FILE *expanded_macros_file;
    int line_num;
    int i;

    *ic = STARTING_IC; // Starting point of assembler

    init_macro_table(&macro_table);
    init_macro_name_array(macroNames);

    rewind(file); // Reset file pointer to the beginning before calling pre_run

    //add file num to expended_macro_file_name to make new file for the assembly input file
    add_number_to_string(expended_macro_file_name, EXPANDED_MACRO_FILE_NAME, sizeof(expended_macro_file_name), file_number);

    //Pre run in order to expand macros from asse,bly input file
    pre_run(&macro_table, macroNames, file, expended_macro_file_name); // Keeps track of the number of encountered macros

    expanded_macros_file = fopen(expended_macro_file_name, "r");
    line_num = 0;
    while (fgets(line, MAX_LINE_LENGTH, expanded_macros_file) != NULL) {
        if (!ignore_line(line)) {
            read_line(line, label_table, ic, dc, lines_array, &macro_table, file_number);
        }
        line_num++;
    }

    fclose(file);
    for(i = 0;i<lines_array->number_of_line;i++){
        InstructionLine *instruction_line = &lines_array->lines[i];
        if(instruction_line->instruction_type == DATA_DIRECTIVE){
            instruction_line->starting_address += *ic;
        }
    }

    // Free allocated memory for macro names
    for (i = 0; i < MAX_MACRO_NAMES; ++i) {
        free(macroNames[i]);
    }

    //Declare final ic and dc in lines array
    lines_array->ic = *ic;
    lines_array->dc = *dc;
}

/**
 * Pre-processes the assembly source file to expand macros.
 * performs the first pass over the assembly source file. It identifies macro definitions, stores them in a `MacroTable`,
 * and expands any macro invocations found in the source code. The expanded code is written to a new output file, which will be used in subsequent
 * assembly processing stages.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param macroNames An array of strings that keeps track of all macro names encountered in the source file.
 * @param file The input assembly source file to be processed.
 * @param new_file_name The name of the output file where the expanded assembly code (with macros expanded) will be written.
 */
void pre_run(MacroTable *macro_table, char **macroNames, FILE *file, char* new_file_name) {
    // Run and expand all macros in the program.
    // Return number of macros
    int is_in_macro = 0;
    int macroCount = 0;
    char macroName[MAX_LABEL_LENGTH];
    Macro macro;
    char line[MAX_LINE_LENGTH];

    erase_file_data(new_file_name);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!ignore_line(line)) {
            if (is_macro_definition_start(line)) {
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
                is_in_macro = handle_macro_definition(file, macro_table, line); // Pass macro_table directly
            } else if (is_macro_invocation(line, macroName, macroNames)) {
                expand_macro(&macro, stdout);
                write_expanded_macros_to_file(macro_table, new_file_name);

            } else {
                if (!is_in_macro) {
                    write_line_to_file(line, new_file_name);
                }
            }
        }
    }

}

/**
 * Adds a new macro to the macro table, expanding the table if necessary.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param new_macro The `Macro` structure to be added to the macro table.
 */
void add_macro(MacroTable *macro_table, Macro new_macro) {
    if (macro_table->count >= macro_table->capacity) {
        macro_table->capacity *= 2;
        macro_table->macros = (Macro *)realloc(macro_table->macros, macro_table->capacity * sizeof(Macro));
        if (!macro_table->macros) {
            fprintf(stderr, "Error: Memory allocation failed while expanding macro table\n");
            exit(EXIT_FAILURE);
        }
    }
    macro_table->macros[macro_table->count++] = new_macro;
}

// Return 1 if macro definition start, 0 if not
int is_macro_definition_start(char *line) {
    return strstr(line, "macr ") != NULL;
}

// Return 1 if macro definition end, 0 if not
int is_macro_definition_end(char *line) {
    return strstr(line, "endmacr") != NULL;
}

//Return if the line represents a macro invocation
int is_macro_invocation(char *line, char *macroName, char **macroNames) {
    int i;
    // Extract potential macro name
    sscanf(line, "%s", macroName);

    // Check if the extracted name is not empty and exists in macroNames
    if (macroName[0] != '\0') {
        for (i = 0; i < MAX_MACRO_NAMES && macroNames[i][0] != '\0'; ++i) {
            if (strcmp(macroName, macroNames[i]) == 0) {
                return 1; // Macro name found in the array
            }
        }
    }

    return 0; // Not a macro invocation or name not found
}

/**
 * Processes a macro definition from the assembly source file and adds it to the macro table.
 * @param file A pointer to the `FILE` object representing the assembly source file.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param firstLine A string containing the first line of the macro definition, typically the line that begins with "%macro".
 * @return Returns `0` if the macro definition is complete, or `1` if the end of the file is reached before completing the macro.
 */
int handle_macro_definition(FILE *file, MacroTable *macro_table, const char *firstLine) {
    Macro macro;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LABEL_LENGTH];

    sscanf(firstLine, "%s", macroName);
    strcpy(macro.name, macroName + 7);  // Skip "%macro"
    macro.lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (is_macro_definition_end(line)) {
            add_macro(macro_table, macro);
            return 0;
        }
        lower_string(line);
        strcpy(macro.body[macro.lineCount++], line);
    }

    add_macro(macro_table, macro);
    return 1;
}

//Insert to file expanded macro
void expand_macro(const Macro *macro, FILE *outputFile) {
    int i;
    for (i = 0; i < macro->lineCount; ++i) {
        fprintf(outputFile, "%s", macro->body[i]);
    }
}

/**
 * Processes a single line of assembly code, handling labels, commands, and directives.
 * @param line The line of assembly code to be processed.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param lines_array A pointer to the LinesArray structure that stores all lines of the source file after processing.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 */
void read_line(char *line, LabelTable *label_table, int *ic, int *dc, LinesArray *lines_array, MacroTable *macro_table, int file_number) {
    char label_name[MAX_LABEL_LENGTH] = "";
    int hasLabel;
    Label *new_label;
    InstructionLine *new_instruction_line;

    line = skip_spaces(line);

    new_instruction_line = init_instruction_line(line);

    hasLabel = find_label(line, label_name);

    if (hasLabel) {
        line = strchr(line, ':') + 1;
//      Skip spaces after .data directive
        while (*line && isspace((unsigned char)*line)) {
            line++;
        }
        new_label = handle_label(new_instruction_line, label_name);
    }

    if (is_data_directive(line)) { // If .data or .String
        handle_directives(line, dc, label_table, ic, file_number, new_instruction_line);

        if(hasLabel){
            new_label->type = DATA_DIRECTIVE;
            new_label->address = *dc;
        }
    }
    else if (is_command(line)) {
        new_instruction_line->starting_address=*ic;

        handle_command(line, label_table, macro_table, new_instruction_line);

        (*ic)+= new_instruction_line->binary_line_count;

        if(hasLabel){
            new_label->type = COMMAND;
            new_label->address = *ic;
        }
    }
    if(hasLabel){
        new_instruction_line->label = new_label;
        add_new_label(label_table, new_label);
    }

    if(new_instruction_line->instruction_type==DATA_DIRECTIVE){
        if(new_instruction_line->directive->type == EXTERN || new_instruction_line->directive->type == ENTRY){
            return;
        }
    }
    addInstructionLine(lines_array, new_instruction_line);
}

/**
 * @brief Allocates memory and initializes a new label for the current instruction line.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the label will be associated with.
 * @param label_name The name of the label to be assigned to the new `Label` structure.
 * @return A pointer to the newly created `Label` structure.
 */
Label* handle_label(InstructionLine *new_instruction_line, char* label_name){
    Label *new_label;
    new_label = (Label *)malloc(10 * sizeof(Label));
    if (new_label == NULL) {
        fprintf(stderr, "Memory allocation error for directive\n");
        exit(EXIT_FAILURE);
    }

    strcpy(new_label->name, label_name);

    new_instruction_line->is_label=1;
    new_instruction_line->label = new_label;

    new_label->is_extern=0;
    new_label->is_entry=0;

    return new_label;
}

/**
 * Processes a command line from the assembly source, extracting and classifying its operands.
 * The `handle_command` function analyzes a command line from the assembly source code, identifies the command,
 * extracts its operands, and classifies them according to their types.
 * The function updates the `InstructionLine` structure with the command information and calculates the number
 * of binary lines required for the instruction.
 * @param line The line of assembly code containing the command to be processed.
 * @param label_table A pointer to the LabelTable structure that stores all labels encountered in the source file.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the label will be associated with.
 */
void handle_command(char *line, LabelTable *label_table, MacroTable *macro_table, InstructionLine *new_instruction_line) {
    Command *new_command;
    new_command = init_command();
    new_instruction_line->instruction_type=COMMAND;

    sscanf(line, "%s", new_command->command_name);

    get_operands_data_for_command(new_command->command_name, new_command);

    define_operands_from_line(new_command, line);
    switch (new_command->operand_number) {
        case 1:
            define_operand_types(new_command->src_operand, macro_table);
            classify_operand(new_command->src_operand);
            break;
        case 2:
            define_operand_types(new_command->src_operand, macro_table);
            define_operand_types(new_command->dst_operand, macro_table);

            classify_operand(new_command->src_operand);
            classify_operand(new_command->dst_operand);
            break;
    }
    new_instruction_line->binary_line_count = find_number_of_lines_in_binary(new_command);
    new_instruction_line->command = new_command;
}

// Get all operands from the current line and insert the data to operands in new_command
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

// Get second operand from line (if exists)
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

/**
 * Defines the type of an operand based on its value.
 * @param operand A pointer to the `Operand` structure that contains the operand's value and type.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 */
void define_operand_types(Operand *operand, MacroTable *macro_table){
    int length;

    operand->type = INVALID;
    if (operand->value[0] == '#') {
        // Check if the rest is a valid integer
        length = check_if_valid_integer(operand->value + 1);

        if(length){
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

    else if(is_valid_label(operand->value, macro_table)){
        operand->type = LABEL;
    }
    else{
        fprintf(stderr, "Not valid operand type. %s\n", operand->value);
        exit(EXIT_FAILURE);
    }
}


// Function to check if a string is a valid label name
int is_valid_label(const char *label, MacroTable *macro_table) {
    int i;
    size_t length;
    //TODO - add exit when not valid
   
    //Check length of the label
    length = strlen(label);
    if (length == 0 || length > MAX_LABEL_LENGTH) {
        return 0;
    }

    //check if not assembly keyword
    if(is_known_assembly_keyword(label)){
        return 0;
    }

    //check if Label matches an existing macro name
    for (i = 0; i < macro_table->count; i++) {
        if (strcmp(label, macro_table->macros[i].name) == 0) {
            return 0; // Label matches an existing macro name
        }
    }

    //Check if the first character is a letter
    if (!isalpha((unsigned char)label[0])) {
        return 0;
    }

    //  Check if other characters are letters/numbers
    for (i = 1; i < length; ++i) {
        if (!isalnum((unsigned char)label[i])) {
            return 0;
        }
    }

    return 1; // Valid label
}

// Get number of binary lines for command based on the classification types of the operands
int find_number_of_lines_in_binary(Command *new_command){
    int number_of_binary_lines = new_command->operand_number + 1; //The default for number of binary lines for data word is the operand number

    //If both operands are indirective or directive register classification, then they share the same data word
    if(new_command->operand_number == 2){
        if((new_command->src_operand->classification_type==INDIRECT_REGISTER && new_command->dst_operand->classification_type==INDIRECT_REGISTER) ||
                (new_command->src_operand->classification_type==DIRECT_REGISTER && new_command->dst_operand->classification_type==DIRECT_REGISTER)){
            number_of_binary_lines -= 1;
        }
    }

    return number_of_binary_lines;
}

//classify the operand classification mode
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

        remove_first_character(new_operand->value);

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

// Get the number of operands and opcode for the given command
void get_operands_data_for_command(char* command_name, Command *new_command){
    int i;
    int command_opcode;
    for (i = 0; i < COMMANDS_COUNT; i++) {
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

// Ignore line when going through the assembly line when a comment ';' or an empty line
int ignore_line(char *line) {
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

//Skip spaces in line
char* skip_spaces(char *line) {
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

// Function to check if a line is a label
int find_label(char *line, char *label) {
    char *start;
    size_t len;
    // Skip leading whitespace
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    start = line;

    // Find the end of the label (until space or colon)
    while (*start && !isspace((unsigned char)*start) && *start != ':') {
        start++;
    }
    // If a colon is found, we assume it is a label
    if (*start == ':') {
        len = start - line;
        strncpy(label, line, len);
        label[len] = '\0'; // Null-terminate the label

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
    int i;
    //TODO - Know if label or macro
    // Copy commands into all_instructions
    for (i = 0; i < COMMANDS_COUNT; ++i) {
        if (strcmp(label, COMMANDS[i]) == 0) {
            fprintf(stderr, "Label is not valid -- A known assembly keyword\n");
            return 1; //Label is a known command
        }
    }

    for (i = 0; i < DIRECTIVES_COUNT; ++i) {
        if (strcmp(label, DIRECTIVES[i]) == 0) {
            fprintf(stderr, "Label is not valid -- A known assembly keyword\n");
            return 1; //Label is a known directive
        }
    }

    return 0;
}

// Function to check if a line is a command
int is_command(char *line) {
    int i;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (i = 0; i < COMMANDS_COUNT; i++) {
        if (strncmp(line, COMMANDS[i], strlen(COMMANDS[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

//Check if given label name already exists in label_table
int label_exists(LabelTable *label_table, char *label_name) {
    int i;
    for (i = 0; i < label_table->size; ++i) {
        if (strcmp(label_table->labels[i].name, label_name) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if the line contains a data directive
int is_data_directive(char *line) {
    int i;
    size_t len;
    // Skip leading spaces
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    // Check if the line starts with a dot
    if (*line == '.') {
        line++;
        // Iterate over the directives array and check if any directive matches
        for (i = 0; i < sizeof(DIRECTIVES) / sizeof(DIRECTIVES[0]); i++) {
            len = strlen(DIRECTIVES[i]);
            if (strncmp(line, DIRECTIVES[i], len) == 0 && isspace((unsigned char)line[len])) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Processes an assembly directive and updates the instruction line accordingly.
 * @param line The line of assembly code containing the directive to be processed.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param label_table A pointer to the LabelTable structure that stores all labels encountered in the source file.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the label will be associated with.
 */
void handle_directives(char *line, int *dc, LabelTable *label_table, int* ic, int file_number, InstructionLine *new_instruction_line) {
    char directive_type[MAX_LINE_LENGTH];
    Directive *new_directive;
    new_directive = init_directive();

    new_instruction_line->instruction_type=DATA_DIRECTIVE;
    new_instruction_line->starting_address=*dc;

    // Read the directive type from the line
    if (sscanf(line, "%s", directive_type) != 1) {
        fprintf(stderr, "Error: Failed to read directive type\n");
        new_directive->type = NOT_DIRECTIVE;
        exit(EXIT_FAILURE);
    }

    if (strcmp(directive_type, ".data") == 0) {
        handle_data_directive(line, new_directive, new_instruction_line);

    } else if (strcmp(directive_type, ".string") == 0) {
        handle_string_directive(line, new_directive, new_instruction_line);

    } else if (strcmp(directive_type, ".extern") == 0){
        handle_extern_directive(line, new_directive, label_table, file_number, ic);


    } else if (strcmp(directive_type, ".entry") == 0) {
        handle_entry_directive(new_directive, file_number, label_table, line);

    } else {
            new_directive->type = NOT_DIRECTIVE;
            fprintf(stderr, "Error: Unknown directive %s\n", directive_type);
            exit(EXIT_FAILURE);
    }

    (*dc) += new_instruction_line->binary_line_count;
    new_instruction_line->directive = new_directive;
}

//Mark label as entry in label table and add to entries file
void handle_entry_directive(Directive *new_directive, int file_number, LabelTable *label_table, char* line){
    char *ptr = line;
    Label* label;
    new_directive->type = ENTRY;
    extract_word_after_keyword(ptr, new_directive->label, ".entry");
    label = find_label_by_name(label_table, new_directive->label);
    label->is_entry=1;

    add_entry_to_entries_file(new_directive->label, file_number, label->address);
}

//Add extern label to label table and add to externals file
void handle_extern_directive(char *line, Directive *new_directive, LabelTable *label_table, int file_number, int *ic){
    Label* label;
    char *ptr = line;
    new_directive->type = EXTERN;

    label = (Label *)malloc(10 * sizeof(Label));
    if (label == NULL) {
        fprintf(stderr, "Memory allocation error for directive\n");
        exit(EXIT_FAILURE);
    }
    extract_word_after_keyword(ptr, label->name, ".extern");
    strcpy(new_directive->label, label->name);

    label->address = 0; // external address, will be filled by linker
    label->type = EXTERN_DIRECTIVE;
    label->is_extern=1;
    add_new_label(label_table, label);
    add_extern_to_externals_file(label, file_number, ic);
}

/**
 * Processes a `.string` directive and extracts the string value into the directive structure.
 * @param line The line of assembly code containing the `.string` directive.
 * @param new_directive A pointer to the `Directive` structure to be populated with the extracted string data.
 * @param instruction_line A pointer to the `InstructionLine` structure that will be updated with the binary line count for the string.
 */
void handle_string_directive(char *line, Directive *new_directive, InstructionLine *instruction_line) {
    char *start;
    char *end;
    size_t length;
    new_directive->type = STRING;

    // Find the first quote
    start = strchr(line, '\"');
    if (start) {
        // Find the closing quote
        end = strchr(start + 1, '\"');
        if (end) {
            // Calculate the length of the string inside the quotes
            length = end - start - 1;

            // Allocate memory for the values array
            new_directive->value = (char **)malloc(sizeof(char *) * 2); // One for the string and one for NULL terminator
            if (new_directive->value == NULL) {
                perror("Unable to allocate memory for values");
                exit(EXIT_FAILURE);
            }

            // Allocate memory for the string and copy it
            new_directive->value[0] = (char *)malloc(length + 1);
            if (new_directive->value[0] == NULL) {
                perror("Unable to allocate memory for string");
                exit(EXIT_FAILURE);
            }
            strncpy(new_directive->value[0], start + 1, length);
            new_directive->value[0][length] = '\0'; // Null-terminate the string

            // Set the second element to NULL
            new_directive->value[1] = NULL;
            new_directive->data_values_count = 1;
            instruction_line->binary_line_count=length + 1;

        }
    }
}

/**
 * Processes a `.data` directive and extracts the integer values into the directive structure.
 * @param line The line of assembly code containing the `.data` directive.
 * @param new_directive A pointer to the `Directive` structure to be populated with the extracted data values.
 * @param instruction_line A pointer to the `InstructionLine` structure that will be updated with the binary line count for the data.
 */
void handle_data_directive(char *line, Directive *new_directive, InstructionLine *instruction_line) {
    char *ptr;
    char *values[MAX_LINE_LENGTH];
    size_t values_count;
    int i;
    char *end;
    int number;
    char buffer[12]; // Buffer to hold the string representation of the number

    new_directive->type = DATA;
    // Move the pointer to the first character after ".data"
    ptr = line + strlen(".data");

    values_count = 0;

    while (*ptr != '\0') {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == ',') {
            ptr++;
        }
        if (isdigit(*ptr) || (*ptr == '-' && isdigit(*(ptr + 1))) || (*ptr == '+' && isdigit(*(ptr + 1)))) {
            number = strtol(ptr, &end, 10);
            ptr = end; // Move ptr to the end of the parsed number

            // Convert number to string and store in values
            snprintf(buffer, sizeof(buffer), "%d", number);

            values[values_count] = strdup(buffer);
            values_count++;
        } else {
            ptr++;
        }

    }

    // Allocate memory for the values in the directive
    new_directive->value = (char **)malloc(values_count * sizeof(char *));
    if (new_directive->value == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Copy the values to the directive
    for (i = 0; i < values_count; i++) {
        new_directive->value[i] = values[i];
    }

    new_directive->data_values_count = values_count;

    instruction_line->binary_line_count = values_count;
}


//Return the label with the given name
Label *find_label_by_name(LabelTable* label_table, char* label_name){
    int i;
    for(i=0;i<label_table->size;i++){
        if(strcmp(label_table->labels[i].name, label_name)){
            return &label_table->labels[i];
        }
    }
    fprintf(stderr, "Error finding label %s", label_name);
    exit(EXIT_FAILURE);
}
