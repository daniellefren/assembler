#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../include/second_run.h"



#define BINARY_INSTRUCTION_MAX_LENGTH 46
#define BINARY_LINE_LENGTH 15

void start_second_run(LinesArray *assembly_lines_array){
    printf("Starting Second run \n");
    printf("The number of lines in the struct is %d \n \n", assembly_lines_array->number_of_line);
    printf("Starting to print The lines Binary: \n");

    for (int i = 0; i < assembly_lines_array->number_of_line; ++i){
        printf("\n****Line number %d *****  \n\n", i);
        InstructionLine *p_line = &assembly_lines_array->lines[i];
        allocate_binary_instruction(p_line, p_line->binary_line_count, BINARY_LINE_LENGTH);
        print_instruction_line(p_line);
        fill_instruction_line_binary(p_line);

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
    int binary_line_count = instruction_line->binary_line_count;
    if (is_instruction_line_directive(*instruction_line)){ //TODO delete after danielle add logic for count directive
        instruction_line->binary_line_count = 10;
        binary_line_count = 10;
    }
    else if (binary_line_count == 0){
        fprintf(stderr, "Error: 0 lines of binary for the instruction line\n");
        return;
    }
    printf("The number of lines in the binary is: %d \n", binary_line_count);
    fill_the_binary_with_zero(binary_instruction_p, BINARY_LINE_LENGTH*binary_line_count);

    if (is_instruction_line_opcode(*instruction_line)){
        fill_first_part_binary_opcode(instruction_line, binary_instruction_p);
        printf("First part binary -                                      %s \n\n", binary_instruction_p);
        if (binary_line_count >1){
            fill_second_part_binary_opcode(instruction_line, binary_instruction_p);
            printf("Second part binary -                                     %s \n", binary_instruction_p);
        }
    }
    else if(is_instruction_line_directive(*instruction_line)){
        fill_binary_directive(instruction_line, binary_instruction_p);
    }
    printf("***The binary representation of the line is %s \n", instruction_line->binary_instruction);
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
        return;
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
            int_to_binary_string(int_number_to_binary, binary_string, operand_number * BINARY_LINE_LENGTH, 12);
            set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
            break;
            //Label with 12 sivi for address from label table and 3 for ARE R=1 for internal and E=1 for External
        case DIRECT:
            int_number_to_binary = operand->label->address;
            int_to_binary_string(int_number_to_binary, binary_string, operand_number * BINARY_LINE_LENGTH, 12);
            if (operand->label->is_entry) { //internal
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'r');
            } else { // external
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'e');
            }
            break;
        case INDIRECT_REGISTER:
        case DIRECT_REGISTER:
            register_value = operand->value;
            if (operand_number == 1) { //src operand
                if (second_operand != NULL){
                    if ((second_operand->classification_type == INDIRECT_REGISTER) ||
                        (second_operand->classification_type == DIRECT_REGISTER)) { //second operand in register
                        register_to_binary_string(register_value, operand_number, binary_string, BINARY_LINE_LENGTH);
                        register_value = second_operand->value;
                        register_to_binary_string(register_value, operand_number + 1, binary_string,BINARY_LINE_LENGTH);
                        set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a'); //TODO why +1
                    }
                    else{ // src operand with dst direct or immediate
                        register_to_binary_string(register_value, operand_number, binary_string,BINARY_LINE_LENGTH);
                        set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
                    }
                }
            }
            else{ //dst operand
                int a_offset = (BINARY_LINE_LENGTH*operand_number) + 12;
                if (binary_string[a_offset] == '0'){ //The src operand wasn't a register
                    register_to_binary_string(register_value, operand_number, binary_string,BINARY_LINE_LENGTH);
                    set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
                }
            }
            break;
        case METHOD_UNKNOWN:
            break;

    }
}
void register_to_binary_string(char *register_value, int operand_number, char *binary_string, int offset){
    //src its 8-6 and dst its 3-5 012345 678 91011 121314
    int register_number;
    register_number = char_to_int(register_value + 1);
    if (operand_number == 2) { //dst bit 91011
        int_to_binary_string(register_number, binary_string,offset +9, 3);
    }
    else if(operand_number == 1) {//src bit 678
        int_to_binary_string(register_number, binary_string,offset +6, 3);
    }
}


void fill_binary_directive(InstructionLine *instruction_line, char *binary_string){
    int number_value;
    Directive *directive = instruction_line->directive;

    if (is_directive_data(directive)){
        for (int i = 0; i < directive->data_values_count; ++i) {
            if (directive->value == NULL){
                fprintf(stderr, "Error - trying to access a directive->value but the value is NULL\n");
                return;
            }
            if (directive->value[i] == NULL){
                fprintf(stderr, "Error - trying to access a directive->value[i] but the value is NULL\n");
                return;
            }
            number_value = char_to_int(directive->value[i]);
            int_to_binary_string(number_value, binary_string,i *BINARY_LINE_LENGTH, BINARY_LINE_LENGTH);
        }
    }

    else if (is_directive_string(directive)){
        for (int i = 0; i < directive->data_values_count - 1; ++i) {
            if (directive->value == NULL){
                fprintf(stderr, "Error - trying to access a directive->value but the value is NULL\n");
                return;
            }
            if (directive->value[i] == NULL){
                fprintf(stderr, "Error - trying to access a directive->value[i] but the value is NULL\n");
                return;
            }
            char_to_binary_string(directive->value[0][i], binary_string,i*BINARY_LINE_LENGTH, BINARY_LINE_LENGTH);
        }
    }

}

void int_to_binary_string(int num, char *binary_string, int offset, int num_bits) {
    int i;

    // Handle negative numbers
    if (num < 0) {
        binary_string[offset] = '1';
        num = -num;
    } else {
        binary_string[offset] = '0';
    }

    // Convert to binary
    for (i = 0; i < num_bits; i++) {
        binary_string[offset + num_bits - i - 1] = (num & (1 << i)) ? '1' : '0';
    }
}

void char_to_binary_string(char c, char *binary_string, int offset, int num_bits) {
    int i;

    // Ensure binary_string is large enough to hold the result
    if (binary_string == NULL || offset + num_bits > CHAR_BIT * sizeof(char) * strlen(binary_string)) {
        fprintf(stderr, "Invalid binary string buffer\n");
        return;
    }

    // Convert character to unsigned char for positive representation
    unsigned char uc = (unsigned char) c;

    // Convert to binary
    for (i = offset + num_bits - 1; i >= offset; i--) {
        binary_string[i] = (uc & 1) + '0';
        uc >>= 1;
    }
}
bool instruction_line_has_three_binary_words(InstructionLine instructionLine) {
    return false;
}

int is_operand_classification_type_valid(enum operand_classification_type operandClassificationType){
    return (operandClassificationType >= IMMEDIATE) && (operandClassificationType <= DIRECT_REGISTER);
}
