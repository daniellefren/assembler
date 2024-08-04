#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <stdbool.h>
#include "../include/second_run.h"



#define BINARY_INSTRUCTION_MAX_LENGTH 46
#define BINARY_LINE_LENGTH 15

void start_second_run(LinesArray *assembly_lines_array){
    printf("Starting Second run \n");


    //Allocation
    assembly_lines_array = malloc(sizeof(LinesArray));

    //assembly_lines_array = generate_instruction_line_array(2, assembly_lines_array);

    printf("The number of lines in the struct is %d \n \n", assembly_lines_array->number_of_line);

    printf("Starting to print The lines Binary: \n");

    for (int i = 0; i < assembly_lines_array->number_of_line; ++i) {
        InstructionLine *p_line = &assembly_lines_array->lines[i];
        allocate_binary_instruction(p_line, p_line->binary_line_count, BINARY_LINE_LENGTH);
        printf("Line number %d binary -  \n", i);
        fill_instruction_line_binary(p_line);
        printf("The binary representation of the line is %s \n", assembly_lines_array->lines[i].binary_instruction );
    }

    for (int j = 0; j < assembly_lines_array->number_of_line; ++j) {
        free_binary_instruction(&assembly_lines_array->lines[j]);
    }

    free(assembly_lines_array[0].lines);
    free(assembly_lines_array);

}


void fill_instruction_line_binary(InstructionLine *instruction_line){
    // check if it's an opcode or directive if it's an opcode
    // create first word binary (15)
    // check how many word needed
    // create second word and third word if needed binary (15 + 15)
    // append the binary words
    // if it's directive .....
    char *binary_instruction_p = instruction_line->binary_instruction;
    if (is_instruction_line_opcode(*instruction_line)){
        char binary_line[(instruction_line->binary_line_count * BINARY_LINE_LENGTH) + 1];
        if (instruction_line->binary_line_count == 2){

        }
        else{

        }

        fill_first_part_binary_opcode(instruction_line, instruction_line->binary_instruction);
        printf("First part binary - %s \n", instruction_line->binary_instruction);

        fill_second_part_binary_opcode(instruction_line, instruction_line->binary_instruction);
        printf("Second part binary - %s \n", instruction_line->binary_instruction);

        // if the line doesn't have three word it has to have two words because it's an opcode line

    }
    else if(is_instruction_line_directive(*instruction_line)){
        fill_binary_directive(instruction_line, instruction_line->binary_instruction);
    }
}

void fill_first_part_binary_opcode(InstructionLine *instruction_line, char *binary_string) {
    // Convert the opcode value to binary opcode - 0 1 2 3, first_classification - 4 5 6 7, second_classification - 8 9 10 11, ARE - 12 13 14
    int first_operand_classification = instruction_line->command->src_operand->classification_type;
    int second_operand_classification = instruction_line->command->dst_operand->classification_type;
    int binary_string_number = 1; //first part binary


    fill_the_binary_representation_with_zero(binary_string, BINARY_LINE_LENGTH);
    printf("The binary string is: %s \n", binary_string);
    set_binary_string_opcode_representation(instruction_line->command->opcode_command_type, *binary_string);
    printf("The opcode binary string is: %c \n", binary_string[6]);
    set_binary_string_operand_representation(first_operand_classification, second_operand_classification, *binary_string);
    set_binary_string_ARE_representation(*binary_string,binary_string_number);
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
    return false;
    //return instructionLine.directive_type != NOT_DIRECTIVE;
}

bool is_instruction_line_opcode(InstructionLine instructionLine){
    return true;
    //return instructionLine.directive_type != NOT_OPCODE;
}





void fill_second_part_binary_opcode(InstructionLine *instruction_line, char* binary_string) {
    // The second part can be 15 bit or 30 bit it depend on the content of the line
    size_t size_of_binary = instruction_line->binary_line_count - 1;
    binary_string = "1111111111111111111 \n";
}

void fill_binary_directive(InstructionLine *instruction_line, char *binary_string){
    for (int i = 0; i < instruction_line->directive->data_values_count; ++i) {
        if (is_instruction_line_directive_string(instruction_line)){
            char_to_binary_string(instruction_line->directive->value[i], binary_string, i * BINARY_LINE_LENGTH);
        }
        else if (is_instruction_line_directive_integer(instruction_line)){
            //TODO fill_binary_integer_data()
        }

    }
}
void char_to_binary_string(char c, char *binary_string, int start_point) {
    int i;
    char binary_str_tmp[BINARY_LINE_LENGTH + 1];
    // Ensure binary_str is large enough (at least 16 characters)
    if (binary_string == NULL || strlen(binary_string) < 15) {
        fprintf(stderr, "Invalid binary string buffer\n");
        return;
    }
    /* TODO - clear the binary str at start and the i dont need to memset at the function
    // Clear the binary string
    memset(binary_string, '0', 15);
    binary_str[15] = '\0';
    */
    memset(binary_str_tmp, '0', 15);
    binary_str_tmp[15] = '\0';

    // Convert character to integer
    int value = (int)c;

    // Convert integer to binary string
    for (i = 14; i >= 0; i--) {
        binary_str_tmp[i] = (value & 1) + '0';
        value >>= 1;
    }
    strcpy(binary_string + start_point, binary_str_tmp);
}

bool is_instruction_line_directive_integer(InstructionLine *instruction_line){
    return false;
}

bool is_instruction_line_directive_string(InstructionLine *instruction_line){
    return true;
}

bool instruction_line_has_three_binary_words(InstructionLine instructionLine) {
    return false;
}

