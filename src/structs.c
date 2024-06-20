#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/structs.h"


/* Lines array fuctions*/

LinesArray *init_line_array(LinesArray *lines_array_pointer){
    return NULL;
}

void free_lines_array(LinesArray *lines_array_pointer){
    printf("a");
}


/* Instruction lines fuctions*/
InstructionLine *init_instruction_line(InstructionLine *instruction_line_pointer){
    return NULL;
}


void free_instruction_line(InstructionLine *instruction_line_pointer){
    printf("a");
}


LinesArray *generate_instruction_line_array(LinesArray *my_lines_array){
    int max_lines = 25;

    my_lines_array->lines = (InstructionLine *) malloc((max_lines) * sizeof(InstructionLine));

    if (my_lines_array->lines == NULL) {
    printf("Memory allocation failed!\n");
    return NULL;
    }

    // Initialize other members of LinesArray
    my_lines_array->number_of_line = 0;
    my_lines_array->capacity = max_lines;

    // Add test assembly instructions (assuming you have a function to create InstructionLine)
    InstructionLine instruction;

    instruction.line_content = "jsr fn1";
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;

    instruction.line_content = "LOOP:";
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;


    return my_lines_array;
}