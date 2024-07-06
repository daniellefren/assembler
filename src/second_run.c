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

        printf("The binary representation of the line is %s \n", assembly_lines_array->lines[i].binary_instruction );
    }
    free(assembly_lines_array[0].lines);
    free(assembly_lines_array);

}


char *return_instruction_line_in_binary(InstructionLine instruction_line){
    // check if it's an opcode or directive if it's an opcode
    // create first word binary (15)
    // check how many word needed
    // create second word and third word if needed binary (15 + 15)
    // append the binary words
    // if it's directive .....

    if (is_instruction_line_opcode(instruction_line)){
        char first_part_binary[16];
        char second_part_binary[31];
        create_first_part_binary_from_instruction_line_opcode(instruction_line, first_part_binary, sizeof(first_part_binary));
        printf("First part binary - %s \n", first_part_binary);


        create_second_part_binary_from_instruction_line_opcode(instruction_line, second_part_binary, sizeof(second_part_binary));
        printf("Second part binary - %s \n", second_part_binary);
        return second_part_binary;
        // if the line doesn't have three word it has to have two words because it's an opcode line

    }
    else if(is_instruction_line_directive(instruction_line)){

    }
    return 'd';
}

void create_first_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char *binary_string, int size_of_binary) {
    // Convert the opcode value to binary opcode - 0 1 2 3, first_classification - 4 5 6 7, second_classification - 8 9 10 11, ARE - 12 13 14
    int first_operand_classification = instruction_line.first_operand_classification_type;
    int second_operand_classification = instruction_line.second_operand_classification_type;
    int binary_string_number = 1; //first part binary


    fill_the_binary_representation_with_zero(binary_string, size_of_binary);
    printf("The binary string is: %s \n", binary_string);
    set_binary_string_opcode_representation(instruction_line.opcode_command_type, binary_string);
    printf("The opcode binary string is: %c \n", binary_string[6]);
    set_binary_string_operand_representation(first_operand_classification, second_operand_classification, binary_string);
    set_binary_string_ARE_representation(binary_string,binary_string_number);
}
void set_binary_string_operand_representation(int first_operand_classification_type, int second_operand_classification_type, char *binary_string) {

    int operand_binary_classification_size = 4;
    int first_classification_offset = 3;
    int second_classification_offset = 7;

    printf("First classification type is %d \n", first_operand_classification_type);
    printf("second classification type is %d \n", second_operand_classification_type);
    if ((first_operand_classification_type < DIRECT_REGISTER) && (first_operand_classification_type > IMMEDIATE)) {
        //bit 5 = method 3, bit 6 method 2, bit 7 method 1, bit 8 method 0
        binary_string[first_classification_offset + operand_binary_classification_size - first_operand_classification_type] = '1';
    }
    if ((second_operand_classification_type < DIRECT_REGISTER) && (second_operand_classification_type > IMMEDIATE)) {
        //bit 9 = method 3, bit 10 method 2, bit 11 method 1, bit 12 method 0
        binary_string[second_classification_offset + operand_binary_classification_size - second_operand_classification_type] = '1';
    }
    printf("The binary string with operand is: %s \n", binary_string);
}

void set_binary_string_ARE_representation(char *binary_string, int number_of_binary_string){
    //number of binary string - is it the first binary string, second or third
    if (number_of_binary_string == 1){
        binary_string[12] = '1';
    }
    return;
}


void fill_the_binary_representation_with_zero(char *binary_string, size_t length) {
    // Check if length is valid (avoid buffer overflow)
    if (length <= 0) {
        return;
    }

    // Fill the array with '0' characters using a loop
    for (size_t i = 0; i < length - 1; i++) {
        binary_string[i] = '0';
    }

    // Add null terminator at the end (within valid memory)
    binary_string[length - 1] = '\0';

}
void set_binary_string_opcode_representation(int opcode_number, char *binary_string) {
    printf("The opcode is %d \n", opcode_number);
    int binary_value = opcode_number & ((1 << OPCODE_SIZE) - 1);  // Mask with all bits set for the desired size

    // Convert each bit to a character ('0' or '1')
    for (int i = 0; i < OPCODE_SIZE; i++) {
        if (binary_value & (1 << (OPCODE_SIZE - 1 - i))) {
            binary_string[i] = '1';
        }
    }
    printf("The opcode binary string is: %s \n", binary_string);
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




void create_second_part_binary_from_instruction_line_opcode(InstructionLine instruction_line, char* binary_string, int size_of_binary) {
    // The second part can be 15 bit or 30 bit it depend on the content of the line
    binary_string = "1111111111111111111 \n";
}


bool instruction_line_has_three_binary_words(InstructionLine instructionLine) {
    return false;
}

