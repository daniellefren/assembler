#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <stdbool.h>
#include "../include/structs.h"
#include "../include/second_run.h"

int second_run(){
    printf("enter \n");
    LinesArray *my_line_array;
    printf("Size of LinesArray: %lu\n", sizeof(LinesArray));
    my_line_array = malloc(sizeof(LinesArray));
    fprintf(stderr,"Something went horribly awry!\n");
    fflush(stderr);
    my_line_array = generate_instruction_line_array(25, my_line_array);
    fprintf(stderr,"Something went horribly awry!\n");
    fflush(stderr);
    printf("The number of lines in the struct is %d \n", my_line_array->number_of_line);
    for (int i = 0; i < my_line_array->number_of_line; ++i) {
        my_line_array->lines[i].binary_instruction = return_instruction_line_in_binary(my_line_array->lines[i]);
        //printf("The binary is %s \n", my_line_array->lines[i].binary_instruction);
        fprintf(stderr,"Something went horribly awry!\n");
        fflush(stderr);
    }

    free(my_line_array[0].lines);
    free(my_line_array);
    return 1;
}

bool is_instruction_line_directive(InstructionLine instructionLine){
    return instructionLine.directive_type != NOT_DIRECTIVE;
}
bool is_instruction_line_opcode_command(InstructionLine instructionLine){
    return instructionLine.directive_type != NOT_OPCODE;
}

char *string_append(char *s1, char *s2) {
    int s1_length = strlen(s1);
    int s2_length = strlen(s2);
    int size = s1_length + s2_length + 1;
    char *s = calloc(size, sizeof(char));  // Allocate enough space for combined string + null terminator

    if (s == NULL) {
        // Handle memory allocation failure (e.g., return NULL)
        return NULL;
    }

    strcpy(s, s1);
    strcat(s, s2);

    return s;
}

char *get_opcode_binary_representation(int opcode, char* binary_string) {
    if (opcode < 0 || opcode > 15) {
        // Handle invalid opcode values
        return NULL;  // Or return an error message
    }
    int max_bits = 4;  // Assuming opcodes are 4 bits wide
    if (binary_string == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    for (int i = max_bits - 1; i >= 0; i--) {
        binary_string[i] = (opcode & (1 << i)) ? '1' : '0';
        opcode = opcode >> 1;  // Right shift by 1 to check next bit
    }

    binary_string[max_bits] = '\0';  // Add null terminator

    return binary_string;
}
char *create_first_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string) {

    binary_string = get_opcode_binary_representation(instruction_line.opcode_command_type, binary_string);
    return binary_string; // Replace with actual binary string
}

char *create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line) {
    // The second part can be 15 bit or 30 bit it depend on the content of the line
    return "placeholder_second_word";
}


bool instruction_line_has_three_binary_words(InstructionLine instructionLine) {
    return false;
}


char *return_instruction_line_in_binary(InstructionLine instruction_line){
    // check if it's an opcode or directive if it's an opcode
    // create first word binary (15)
    // check how many word needed
    // create second word and third word if needed binary (15 + 15)
    // append the binary words
    // if it's directive .....

    if (is_instruction_line_opcode_command(instruction_line)){
        char *first_part_binary;
        char *second_part_binary;
        first_part_binary = calloc(16, sizeof(char));  // Allocate space for max bits + null terminator
        first_part_binary = create_first_part_binary_from_instruction_line_opcode(instruction_line, first_part_binary);
        printf("%s", first_part_binary);
        free(first_part_binary);
        second_part_binary = create_second_part_binary_from_instruction_line_opcode(instruction_line);
        // if the line doesn't have three word it has to have two words because it's an opcode line

    }
    else if(is_instruction_line_directive(instruction_line)){

    }
    return 'd';
}