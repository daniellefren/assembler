#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <stdbool.h>
#include "../include/second_run.h"



#define BINARY_INSTRUCTION_MAX_LENGTH 46
#define BINARY_LINE_LENGTH 15

void start_second_run(LinesArray *assembly_lines_array){
    printf("Starting Second run \n");
    printf("The number of lines in the struct is %d \n \n", assembly_lines_array->number_of_line);
    printf("Starting to print The lines Binary: \n");

    //for (int i = 0; i < assembly_lines_array->number_of_line; ++i)
    for (int i = 0; i < 5; ++i) {
        InstructionLine *p_line = &assembly_lines_array->lines[i];
        allocate_binary_instruction(p_line, p_line->binary_line_count, BINARY_LINE_LENGTH);
        printf("\n****Line number %d *****  \n\n", i);
        print_instruction_line(p_line);
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
    // to create second word and third word if needed binary (15 + 15)
    // append the binary words
    // if it's directive .....
    char *binary_instruction_p = instruction_line->binary_instruction;
    if (is_instruction_line_opcode(*instruction_line)){
        printf("The number of lines in the binary is: %d \n", instruction_line->binary_line_count);
        fill_the_binary_with_zero(binary_instruction_p, BINARY_LINE_LENGTH*instruction_line->binary_line_count);
        fill_first_part_binary_opcode(instruction_line, binary_instruction_p);
        printf("First part binary -                                      %s \n\n", binary_instruction_p);
        fill_second_part_binary_opcode(instruction_line, binary_instruction_p);
        printf("Second part binary -                                     %s \n", binary_instruction_p);


    }
    else if(is_instruction_line_directive(*instruction_line)){
        fill_binary_directive(instruction_line, binary_instruction_p);
    }
}

void fill_first_part_binary_opcode(InstructionLine *instruction_line, char *binary_string) {
    // Convert the opcode value to binary opcode - 0 1 2 3, first_classification - 4 5 6 7, second_classification - 8 9 10 11, ARE - 12 13 14
    int binary_string_number = 1; //first part binary
    int first_operand_classification;
    int second_operand_classification = -1; //so it won't change binary in set_binary_string_operand if there is only one operand

    set_binary_string_opcode_representation(instruction_line->command->opcode_command_type, binary_string);
    set_binary_string_ARE_representation(binary_string,binary_string_number, 'a');
    printf("The binary string with ARE is:                           %s \n", binary_string);
    if (instruction_line->command->operand_number > 0){
        first_operand_classification = instruction_line->command->src_operand->classification_type;
    }
    else{
        printf("The binary string with ARE is: %s \n", binary_string);
        exit(1); //TODO - make sure is dont exit for command with no operand if there is one
    }
    if (instruction_line->command->operand_number >1){
        second_operand_classification = instruction_line->command->dst_operand->classification_type;
    }

    set_binary_string_operand_representation(first_operand_classification, second_operand_classification, binary_string);
    printf("The binary string with operand is:                       %s \n", binary_string);


}


void set_binary_string_operand_representation(int first_operand_classification_type, int second_operand_classification_type, char *binary_string) {

    int operand_binary_classification_size = 4;
    int first_classification_offset = 3; //The offset where the first classification start at the binary
    int second_classification_offset = 7; //The offset where the second classification start at the binary

    printf("First classification type is %d \n", first_operand_classification_type);
    printf("second classification type is %d \n", second_operand_classification_type);
    if (is_operand_classification_type_valid(first_operand_classification_type)) {
        //bit 5 = method 3, bit 6 method 2, bit 7 method 1, bit 8 method 0
        binary_string[first_classification_offset + operand_binary_classification_size - first_operand_classification_type] = '1';
    }
    if (is_operand_classification_type_valid(second_operand_classification_type)) {
        //bit 9 = method 3, bit 10 method 2, bit 11 method 1, bit 12 method 0
        binary_string[second_classification_offset + operand_binary_classification_size - second_operand_classification_type] = '1';
    }
}

void set_binary_string_ARE_representation(char *binary_string, int binary_string_number, char ARE){
    //number of binary string - is it the first binary string, second or third
    //First word always A = 1
    int offset = 11 + (15 * (binary_string_number-1));
    switch (ARE) {
        case 'a':
            offset += 1;
            break;
        case 'r':
            offset += 2;
            break;
        case 'e':
            offset += 3;
            break;
        default:
            printf("Exception Character is not a/r/e\n");
            exit(1);
    }
    binary_string[offset] = '1';
}


void fill_the_binary_with_zero(char *binary_string, size_t length) {
    // Check if length is valid (avoid buffer overflow)
    if (length <= 0) {
        return;
    }
    // Fill the array with '0' characters using a loop
    for (size_t i = 0; i < length; i++) {
        binary_string[i] = '0';
    }

    // Add null terminator at the end (within valid memory)
    binary_string[length] = '\0';
    printf("The binary string (filled with zero) is: %s \n", binary_string);

}
void set_binary_string_opcode_representation(int opcode_number, char *binary_string) {
    printf("The opcode is %d \n", opcode_number);
    if (binary_string == NULL) {
        exit(0);
    }
    int binary_value = opcode_number & ((1 << OPCODE_SIZE) - 1);  // Mask with all bits set for the desired size

    // Convert each bit to a character ('0' or '1')
    for (int i = 0; i < OPCODE_SIZE; i++) {
        if (binary_value & (1 << (OPCODE_SIZE - 1 - i))) {
            binary_string[i] = '1';
        }
    }

    printf("The opcode binary string after opcode representation is: %s \n", binary_string);
}

bool is_instruction_line_directive(InstructionLine instructionLine){
    if (instructionLine.directive != NULL){
        return true;
    }
    return false;
    //return instructionLine.directive_type != NOT_DIRECTIVE;
}

bool is_instruction_line_opcode(InstructionLine instructionLine){
    if (instructionLine.command != NULL){
        return true;
    }
    return false;
    //return instructionLine.directive_type != NOT_OPCODE;
}





void fill_second_part_binary_opcode(InstructionLine *instruction_line, char *binary_string) {
    // The second part can be 15 bit or 30 bit it depend on the content of the line
    int operand_number = instruction_line->command->operand_number;
    Operand *src_operand = instruction_line->command->src_operand;
    Operand *dst_operand = instruction_line->command->dst_operand;

    switch (operand_number) {
        case 2:
            fill_operand_binary(src_operand, dst_operand, binary_string,1);
            fill_operand_binary(dst_operand, NULL, binary_string,2);
            break;
        case 1:
            fill_operand_binary(src_operand, NULL, binary_string,1);
            break;
        case 0:
            break;
        default:
            printf("Exception - problem with operand counter it's %d!", operand_number);

    }
}
void fill_operand_binary(Operand *operand,Operand *second_operand, char *binary_string, int operand_number) {
    //operand number - 1 for src operand and 2 for dst operand
    int int_number_to_binary;
    int binary_string_number;
    char *register_value;
    switch (operand->classification_type) {
        case IMMEDIATE:
            int_number_to_binary = char_to_int(operand->value);
            int_to_binary_string(int_number_to_binary, binary_string, operand_number * BINARY_LINE_LENGTH);
            set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
            break;
            //Label with 12 sivi for address from label table and 3 for ARE R=1 for internal and E=1 for External
        case DIRECT:
            int_number_to_binary = operand->label->address;
            int_to_binary_string(int_number_to_binary, binary_string, operand_number * BINARY_LINE_LENGTH);
            if (operand->label->internal) { //internal
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'r');
            } else if (!operand->label->internal) { // external
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'e');
            } else {
                printf("Exception - Error not a viable label!");
                exit(1);
            }
            break;
        case INDIRECT_REGISTER:
            register_value = operand->value;
            if (second_operand != NULL) { //
                if ((second_operand->classification_type == INDIRECT_REGISTER) ||
                    (second_operand->classification_type == DIRECT_REGISTER)) {
                    if (operand_number == 1) { //Change only on the src operand not on the dst (on the dst it's already changed
                        register_to_binary_string(register_value, operand_number, binary_string, BINARY_LINE_LENGTH);
                        register_value = second_operand->value;
                        register_to_binary_string(register_value, operand_number + 1, binary_string,BINARY_LINE_LENGTH);
                    }
                    set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
                }
            }
            else{
                register_to_binary_string(register_value, operand_number, binary_string,BINARY_LINE_LENGTH);
            }
            break;
        case DIRECT_REGISTER:
            break;
            set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
        case METHOD_UNKNOWN:
            break;

    }
}
void register_to_binary_string(char *register_value, int operand_number, char *binary_string, int offset){

}

void int_to_binary_string(int num, char *binary_string, int offset) {
    // Convert the number to binary and populate the string
    int i;
    for (i = 0; i < 12; i++) {
        binary_string[offset + 11 - i] = (num & (1 << i)) ? '1' : '0';
    }
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

int is_operand_classification_type_valid(enum operand_classification_type operandClassificationType){
    return (operandClassificationType >= IMMEDIATE) && (operandClassificationType <= DIRECT_REGISTER);
}
