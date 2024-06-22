#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/structs.h"


/* Lines array functions*/

LinesArray *init_line_array(LinesArray *lines_array_pointer){
    return NULL;
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

    instruction.line_content = "jsr fn1";
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;

    instruction.line_content = "LOOP:";
    instruction.length = strlen(instruction.line_content);
    my_lines_array->lines[my_lines_array->number_of_line++] = instruction;

    return my_lines_array;
}

