#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/structs.h"
#include "../include/errors.h"


/* Lines array functions*/

LinesArray *init_lines_array(int initial_capacity) {
    LinesArray *lines_array = (LinesArray *)malloc(sizeof(LinesArray));
    if (!lines_array) {
        print_internal_error(ERROR_CODE_46, "");
        exit(EXIT_FAILURE);
    }
    lines_array->lines = (InstructionLine *)malloc(initial_capacity * sizeof(InstructionLine));
    if (!lines_array->lines) {
        print_internal_error(ERROR_CODE_15, "");
        exit(EXIT_FAILURE);
    }
    lines_array->number_of_line = 0;
    lines_array->capacity = initial_capacity;

    return lines_array;
}

SymbolTable *init_symbol_table(int initial_capacity) {
    SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!symbol_table) {
        print_internal_error(ERROR_CODE_27, "");
        exit(EXIT_FAILURE);
    }
    symbol_table->symbols = (Symbol *)malloc(initial_capacity * sizeof(Symbol));
    if (!symbol_table->symbols) {
        print_internal_error(ERROR_CODE_14, "");
        exit(EXIT_FAILURE);
    }
    symbol_table->size = 0;
    symbol_table->capacity = initial_capacity;

    return symbol_table;
}

void free_lines_array(LinesArray *lines_array) {
    if (lines_array->lines) {
        for (unsigned int i = 0; i < lines_array->number_of_line; i++) {
            free_instruction_line(&lines_array->lines[i]);
        }
        free(lines_array->lines);
    }

}

void free_macro_table(MacroTable *macro_table) {
    free(macro_table->macros);
}


void free_instruction_line(InstructionLine *instruction_line) {

    if (instruction_line->line_content) {
        free(instruction_line->line_content);
    }
    if(instruction_line->is_symbol){
        free(instruction_line->symbol);
    }
    if(instruction_line->instruction_type == COMMAND){
        free(instruction_line->command);
    }
    else if(instruction_line->instruction_type == DATA_DIRECTIVE ||
                instruction_line->instruction_type == EXTERN_DIRECTIVE ||
                instruction_line->instruction_type == ENTRY_DIRECTIVE){
        // Assuming that `directive->value` is allocated dynamically and needs to be freed
        if (instruction_line->directive->value) {
            for (size_t i = 0; i < instruction_line->directive->data_values_count; i++) {
                free(instruction_line->directive->value[i]);
            }
            free(instruction_line->directive->value);
        }
        free(instruction_line->directive);
    }
    if (instruction_line->binary_instruction){
        free(instruction_line->binary_instruction);
    }
}



void free_binary_instruction(InstructionLine *p_line) {
    free(p_line->binary_instruction);
}

void free_symbol_table(SymbolTable *symbol_table) {
    if (symbol_table->symbols != NULL) {
        free(symbol_table->symbols);
    }
}


// Function to add an instruction line to the LinesArray
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line) {
    // Check if the array needs to be resized
    if (lines_array->number_of_line >= lines_array->capacity) {
        // Double the capacity or set an initial capacity if it's zero
        size_t new_capacity = (lines_array->capacity == 0) ? 10 : lines_array->capacity * 2;
        InstructionLine *new_lines = realloc(lines_array->lines, new_capacity * sizeof(InstructionLine));
        if (!new_lines) {
            print_internal_error(ERROR_CODE_15, "");
            exit(EXIT_FAILURE);
        }
        lines_array->lines = new_lines;
        lines_array->capacity = new_capacity;
    }

    // Add the new instruction line to the array
    lines_array->lines[lines_array->number_of_line] = *instruction_line;
    lines_array->number_of_line++;
}

void add_new_symbol(SymbolTable *symbol_table, Symbol *symbol) {
    // Check if the array needs to be resized
    if (symbol_table->size >= symbol_table->capacity) {
        // Double the capacity or set an initial capacity if it's zero
        size_t new_capacity = (symbol_table->capacity == 0) ? 10 : symbol_table->capacity * 2;
        Symbol *new_symbols = realloc(symbol_table->symbols, new_capacity * sizeof(Symbol));
        if (!new_symbols) {
            print_internal_error(ERROR_CODE_15, "");
            exit(EXIT_FAILURE);
        }

        symbol_table->symbols = new_symbols;
        symbol_table->capacity = new_capacity;
    }

    // Add the new instruction line to the array
    symbol_table->symbols[symbol_table->size] = *symbol;
    symbol_table->size++;
}

void init_macro_table(MacroTable *table) {
    table->macros = (Macro *)malloc(10 * sizeof(Macro));
    if (!table->macros) {
        print_internal_error(ERROR_CODE_10, "");
        exit(EXIT_FAILURE);
    }
    table->count = 0;
    table->capacity = 10;
}

InstructionLine *init_instruction_line(char* line){
    // Allocate memory for instruction line
    InstructionLine *new_instruction_line = (InstructionLine *)malloc(sizeof(InstructionLine));
    if (new_instruction_line == NULL) {
        print_internal_error(ERROR_CODE_16, "");
        exit(EXIT_FAILURE);
    }

    new_instruction_line->line_content = strdup(line);

    if (new_instruction_line->line_content == NULL) {
        print_internal_error(ERROR_CODE_17, "");
        free(new_instruction_line);
        exit(EXIT_FAILURE);
    }

    new_instruction_line->length = strlen(line);
    new_instruction_line->binary_line_count=0;

    return new_instruction_line;
}

Command *init_command(){
    Command *new_command = (Command *)malloc(10 * sizeof(Command));
    if (new_command == NULL) {
        print_internal_error(ERROR_CODE_18, "");
        exit(EXIT_FAILURE);
    }
    // Allocate memory for operands
    new_command->src_operand = init_operand();
    new_command->dst_operand = init_operand();


    //Allocate memory for command name
    new_command->command_name = malloc(MAX_COMMAND_LEN * sizeof(char));
    if (new_command->command_name == NULL) {
        print_internal_error(ERROR_CODE_19, "");
        exit(EXIT_FAILURE);
    }

    return new_command;
}

Operand *init_operand(){
    Operand *operand = (Operand *)malloc(sizeof(Operand));
    if (operand == NULL) {
        print_internal_error(ERROR_CODE_20, "");
        free(operand);
        exit(EXIT_FAILURE);
    }

    operand->value = (char *)malloc(MAX_OPERAND_LEN * sizeof(char));
    operand->symbol = (Symbol *)malloc(sizeof(Symbol));

    return operand;
}

Directive *init_directive(){
    Directive *new_directive = (Directive *)malloc(10 * sizeof(Directive));
    if (new_directive == NULL) {
        print_internal_error(ERROR_CODE_18, "");
        exit(EXIT_FAILURE);
    }
    return new_directive;
}

// Initialize macro name array
void init_macro_name_array(char **macroNames) {
    for (int i = 0; i < MAX_MACRO_NAMES; ++i) {
        macroNames[i] = malloc(MAX_SYMBOL_LENGTH * sizeof(char));
        if (!macroNames[i]) {
            print_internal_error(ERROR_CODE_21, "");
            exit(EXIT_FAILURE);
        }
        macroNames[i][0] = '\0'; // Ensure the string is initialized
    }
}


void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length) {
    size_t count_to_allocate;
    // Calculate the total size needed in bytes, adding 1 for the null terminator.
    count_to_allocate= binary_line_count * binary_line_length + 1;
    printf("The binary line count - %lu\n", binary_line_count);

    if (binary_line_count == 0){
        print_internal_error(ERROR_CODE_35, "");
        exit(EXIT_FAILURE);
    }
    p_line->binary_instruction = (char*) malloc(count_to_allocate);
    if (p_line->binary_instruction == NULL) {
        print_internal_error(ERROR_CODE_22, "");
        exit(EXIT_FAILURE);
    }
}


char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line) {
    return linesArray->lines[number_of_line].binary_instruction;
}

bool is_instruction_line_directive(InstructionLine instructionLine){
    // Check if the `directive` field is not NULL
    if (instructionLine.directive != NULL){
        return true;
    }
    return false;
}

bool is_instruction_line_opcode(InstructionLine instructionLine){
    // Check if the `command` field is not NULL
    if (instructionLine.command != NULL){
        return true;
    }
    return false;
}

int is_operand_classification_type_valid(enum operand_classification_type operandClassificationType){
    // Check if the operand classification type is within the valid range
    return (operandClassificationType >= IMMEDIATE) && (operandClassificationType <= DIRECT_REGISTER);
}
