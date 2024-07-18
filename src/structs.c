#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/structs.h"


/* Lines array functions*/

void init_lines_array(LinesArray *lines_array, int initial_capacity) {
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
}

void init_symbol_table(SymbolTable *symbol_table, int initial_capacity) {
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



/* Instruction lines functions*/
InstructionLine *init_instruction_line(InstructionLine *instruction_line_pointer){
    return NULL;
}


void free_instruction_line(InstructionLine *instruction_line_pointer){
    printf("a");
}

#include <stdlib.h>
#include <string.h>  // for strlen

LinesArray *generate_instruction_line_array(int max_lines, LinesArray *my_lines_array) {
    if (my_lines_array == NULL) {
        printf("Memory allocation failed for LinesArray!\n");
        return NULL;
    }

    my_lines_array->lines = (InstructionLine *) malloc(max_lines * sizeof(InstructionLine));

    if (my_lines_array->lines == NULL) {
        free(my_lines_array);  // Free previously allocated memory for LinesArray
        printf("Memory allocation failed for InstructionLine array!\n");
        return NULL;
    }

    // Initialize other members of LinesArray
    my_lines_array->number_of_line = 0;
    my_lines_array->capacity = max_lines;

    // Add test assembly instructions (assuming you have a function to create InstructionLine)
    InstructionLine instruction;

    instruction.line_content = "MOV r2 *r2";
    instruction.first_operand = "r1";
    instruction.first_operand_classification_type = DIRECT_REGISTER;
    instruction.second_operand= "*r2";
    instruction.second_operand_classification_type = INDIRECT_REGISTER;
    instruction.opcode_command_type = MOV;
    instruction.operand_number = 2;
    instruction.instruction_type = IS_COMMAND;
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;

    instruction.line_content = "INC *r1";
    instruction.first_operand = "*r1";
    instruction.first_operand_classification_type = INDIRECT_REGISTER;
    instruction.operand_number = 1;
    instruction.opcode_command_type = INC;
    instruction.instruction_type = IS_COMMAND;
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;

    return my_lines_array;
}

char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line){
    return linesArray->lines[number_of_line].binary_instruction;
}

// Function to add an instruction line to the LinesArray
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line) {
    printf("addInstructionLine");
    // Check if the array needs to be resized
    if (lines_array->number_of_line >= lines_array->capacity) {
        // Double the capacity or set an initial capacity if it's zero
        size_t new_capacity = (lines_array->capacity == 0) ? 10 : lines_array->capacity * 2;
        InstructionLine *new_lines = realloc(lines_array->lines, new_capacity * sizeof(InstructionLine));
        if (!new_lines) {
            fprintf(stderr, "Error: Unable to allocate memory for lines array\n");
            exit(EXIT_FAILURE);
        }
//        for(int i=0;i<lines_array->capacity;i++){
//            new_lines[i] = lines_array->lines[i];
//        }
        lines_array->lines = new_lines;
        lines_array->capacity = new_capacity;
    }

    // Add the new instruction line to the array
    lines_array->lines[lines_array->number_of_line] = *instruction_line;
    lines_array->number_of_line++;
}

void addNewSymbol(SymbolTable *symbol_table, Symbol *symbol) {
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
