#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/structs.h"


/* Lines array functions*/

LinesArray *init_lines_array(int initial_capacity) {
    LinesArray *lines_array = (LinesArray *)malloc(sizeof(LinesArray));
    if (!lines_array) {
        fprintf(stderr, "Error: LinesArray pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    lines_array->lines = (InstructionLine *)malloc(initial_capacity * sizeof(InstructionLine));
    if (!lines_array->lines) {
        fprintf(stderr, "Memory allocation failed for lines array\n");
        exit(EXIT_FAILURE);
    }
    lines_array->number_of_line = 0;
    lines_array->capacity = initial_capacity;

    return lines_array;
}

SymbolTable *init_symbol_table(int initial_capacity) {
    SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!symbol_table) {
        fprintf(stderr, "Error: SymbolTable pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    symbol_table->symbols = (Symbol *)malloc(initial_capacity * sizeof(Symbol));
    if (!symbol_table->symbols) {
        fprintf(stderr, "Memory allocation failed for symbols array\n");
        exit(EXIT_FAILURE);
    }
    symbol_table->size = 0;
    symbol_table->capacity = initial_capacity;

    return symbol_table;
}

void free_lines_array(LinesArray *lines_array) {
    if (lines_array == NULL) {
        return;  // Nothing to free if lines_array is NULL
    }

    if (lines_array->lines != NULL) {
        free(lines_array->lines);  // Free the allocated memory for InstructionLine array
    }

    free(lines_array);  // Free the memory allocated for LinesArray struct itself
}


void free_instruction_line(InstructionLine *instruction_line_pointer){
    printf("a");
}

char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line){
    return linesArray->lines[number_of_line].binary_instruction;
}

// Function to add an instruction line to the LinesArray
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line) {
    // Check if the array needs to be resized
    if (lines_array->number_of_line >= lines_array->capacity) {
        // Double the capacity or set an initial capacity if it's zero
        size_t new_capacity = (lines_array->capacity == 0) ? 10 : lines_array->capacity * 2;
        InstructionLine *new_lines = realloc(lines_array->lines, new_capacity * sizeof(InstructionLine));
        if (!new_lines) {
            fprintf(stderr, "Error: Unable to allocate memory for lines array\n");
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
            fprintf(stderr, "Error: Unable to allocate memory for lines array\n");
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
        fprintf(stderr, "Error: Memory allocation failed for macro table\n");
        exit(EXIT_FAILURE);
    }
    table->count = 0;
    table->capacity = 10;
}

InstructionLine *init_instruction_line(char* line){
    // Allocate memory for instruction line
    InstructionLine *new_instruction_line = (InstructionLine *)malloc(sizeof(InstructionLine));
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
    new_instruction_line->binary_line_count=0;

    return new_instruction_line;
}

Command *init_command(){
    Command *new_command = (Command *)malloc(10 * sizeof(Command));
    if (new_command == NULL) {
        fprintf(stderr, "Memory allocation error for directive\n");
        exit(EXIT_FAILURE);
    }
    // Allocate memory for operands
    new_command->src_operand = init_operand();
    new_command->dst_operand = init_operand();


    //Allocate memory for command name
    new_command->command_name = malloc(MAX_COMMAND_LEN * sizeof(char));
    if (new_command->command_name == NULL) {
        fprintf(stderr, "Memory allocation error for directive\n");
        exit(EXIT_FAILURE);
    }

    return new_command;
}

Operand *init_operand(){
    Operand *operand = (Operand *)malloc(sizeof(Operand));
    if (operand == NULL) {
        fprintf(stderr, "Memory allocation error for operands\n");
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
        fprintf(stderr, "Memory allocation error for directive\n");
        exit(EXIT_FAILURE);
    }
    return new_directive;
}

// Initialize macro name array
void init_macro_name_array(char **macroNames) {
    for (int i = 0; i < MAX_MACRO_NAMES; ++i) {
        macroNames[i] = malloc(MAX_SYMBOL_LENGTH * sizeof(char));
        if (!macroNames[i]) {
            fprintf(stderr, "Error: Memory allocation failed for macro name %d\n", i);
            exit(EXIT_FAILURE);
        }
        macroNames[i][0] = '\0'; // Ensure the string is initialized
    }
}


