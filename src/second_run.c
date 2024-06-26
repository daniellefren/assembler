#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <stdbool.h>
#include "../include/structs.h"
#include "../include/second_run.h"

#define BINARY_INSTRUCTION_MAX_LENGTH 46

void start_second_run(LinesArray *assembly_lines_array){
    printf("Starting Second run \n");
    //Allocation
    assembly_lines_array = malloc(sizeof(LinesArray));

    assembly_lines_array = generate_instruction_line_array(2, assembly_lines_array);

    printf("The number of lines in the struct is %d \n \n", assembly_lines_array->number_of_line);

    printf("Starting to print The lines Binary: \n");
    for (int i = 0; i < assembly_lines_array->number_of_line; ++i) {
        printf("Line number %d binary -  \n", i);
        assembly_lines_array->lines[i].binary_instruction = calloc(31, sizeof(char));
        assembly_lines_array->lines[i].binary_instruction = return_instruction_line_in_binary(assembly_lines_array->lines[i]);

        printf("The binary represantion of the line is %s \n", assembly_lines_array->lines[i].binary_instruction );
    }
    free(assembly_lines_array[0].lines);
    free(assembly_lines_array);

}

bool is_instruction_line_directive(InstructionLine instructionLine){
    return instructionLine.directive_type != NOT_DIRECTIVE;
}

bool is_instruction_line_opcode(InstructionLine instructionLine){
    return instructionLine.directive_type != NOT_OPCODE;
}

void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size) {
    /*
     In calling function
    char appended_string[100];  // Pre-allocate memory for combined string
    string_append(string1, string2, appended_string, sizeof(appended_string_size));
    */
    size_t first_string_size = strlen(first_string);
    size_t second_string_size = strlen(second_String);
    if (first_string_size + second_string_size + 1 > appended_string_size) {
        // Handle insufficient space in destination (e.g., return error code)
        return;
    }

    // Copy s1 and s2 using strcpy and strcat
    strcpy(appended_string, first_string);
    strcat(appended_string, second_String);
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

char *create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string) {
    // The second part can be 15 bit or 30 bit it depend on the content of the line
    return "1111111111111111111";
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

    if (is_instruction_line_opcode(instruction_line)){
        char *first_part_binary;
        char *second_part_binary;

        first_part_binary = (char *) calloc(16, sizeof(char));
        second_part_binary = (char *) calloc(16, sizeof(char));

        first_part_binary = create_first_part_binary_from_instruction_line_opcode(instruction_line, first_part_binary);
        printf("First part binary - %s \n", first_part_binary);


        second_part_binary = create_second_part_binary_from_instruction_line_opcode(instruction_line, second_part_binary);
        printf("Second part binary - %s \n", second_part_binary);
        free(first_part_binary);
        return second_part_binary;
        // if the line doesn't have three word it has to have two words because it's an opcode line

    }
    else if(is_instruction_line_directive(instruction_line)){

    }
    return 'd';
}