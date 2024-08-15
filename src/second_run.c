#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../include/second_run.h"
#include "../include/files_handler.h"
#include "../include/errors.h"


void start_second_run(LinesArray *assembly_lines_array, SymbolTable *symbol_table, char* file_name){
    int i;
    InstructionLine *p_line;

    printf("Starting Second run \n");

    // Iterate over each line in the assembly lines array
    for (i = 0; i < assembly_lines_array->number_of_line; ++i){
        p_line = assembly_lines_array->lines[i];
//        printf("\n****Line number %d Address %d *****  \n\n", i, p_line->starting_address);
        allocate_binary_instruction(p_line, p_line->binary_line_count, BINARY_LINE_LENGTH);
        fill_instruction_line_binary(p_line, symbol_table);
        //print_instruction_line(p_line); you can use it to debug the lines
    }
    create_ob_file(assembly_lines_array, get_filename(file_name));
    printf("Finished Second run!\n\n");
}

void fill_instruction_line_binary(InstructionLine *instruction_line, SymbolTable *symbol_table){
    // Pointer to the binary instruction array within the instruction line
    char *binary_instruction_p;
    int binary_line_count;

    binary_instruction_p = instruction_line->binary_instruction;
    binary_line_count = instruction_line->binary_line_count;
    if (binary_line_count == 0){
        print_internal_error(ERROR_CODE_35, "");
        return;
    }

    // Initialize the binary instruction array with zeros
    fill_the_binary_with_zero(binary_instruction_p, BINARY_LINE_LENGTH * binary_line_count);
    if (is_instruction_line_opcode(instruction_line)){
        fill_first_part_binary_opcode(instruction_line, binary_instruction_p);
        if (binary_line_count > 1){
            fill_second_part_binary_opcode(instruction_line, binary_instruction_p, symbol_table);
        }
    }
    else if (is_instruction_line_directive(instruction_line)){
        fill_binary_directive(instruction_line, binary_instruction_p);
    }
//    printf("The binary representation of the line is %s \n", instruction_line->binary_instruction);
}

void fill_first_part_binary_opcode(InstructionLine *instruction_line, char *binary_string) {
    int binary_string_number = 1; //first part binary
    int first_operand_classification = -1; // Set to -1 to indicate no second operand if only one operand is present
    int second_operand_classification;

    // Convert the opcode value to its binary representation and store it in binary_string
    set_binary_string_opcode_representation(instruction_line->command->opcode_command_type, binary_string);

    // Set the ARE (Addressing, Register, or External) representation in the binary string
    set_binary_string_ARE_representation(binary_string, binary_string_number, 'a');

    if (instruction_line->command->operand_number <= 0) {
        return; // No operands present, exit function

    } else if (instruction_line->command->operand_number > 1){ //2 operand mean src and dst
        first_operand_classification = instruction_line->command->src_operand->classification_type;

    }
    second_operand_classification = instruction_line->command->dst_operand->classification_type; //1 operand can be just dst

    // Convert operand classifications to their binary representation and update the binary string
    set_binary_string_operand_representation(first_operand_classification,
                                             second_operand_classification, binary_string);
}



void set_binary_string_operand_representation(int first_operand_classification_type,
                                              int second_operand_classification_type, char *binary_string) {
    int offset;

    if (is_operand_classification_type_valid(first_operand_classification_type)) {
        //bit 5 = method 3, bit 6 method 2, bit 7 method 1, bit 8 method 0
        offset = FIRST_CLASSIFICATION_OFFSET + OPERAND_BINARY_CLASSIFICATION_SIZE - first_operand_classification_type;
        binary_string[offset] = '1';
    }
    if (is_operand_classification_type_valid(second_operand_classification_type)) {
        //bit 9 = method 3, bit 10 method 2, bit 11 method 1, bit 12 method 0
        offset = SECOND_CLASSIFICATION_OFFSET + OPERAND_BINARY_CLASSIFICATION_SIZE - second_operand_classification_type;
        binary_string[offset] = '1';
    }
}

void set_binary_string_ARE_representation(char *binary_string, int binary_string_number, char ARE) {
    // Calculate the offset based on binary string number
    int offset = ARE_STARTING_OFFSET + (BINARY_WORD_LENGTH * (binary_string_number - 1));

    // Adjust the offset based on ARE character
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
            print_internal_error(ERROR_CODE_36, "");
            exit(EXIT_FAILURE);
    }

    // Set the ARE bit in the binary string
    binary_string[offset] = '1';
}


void fill_the_binary_with_zero(char *binary_string, size_t length) {
    // Check if the length is valid to avoid unnecessary operations or buffer overflows
    size_t i;
    if (length <= 0) {
        print_internal_error(ERROR_CODE_37, "");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < length; i++) {
        binary_string[i] = '0';
    }
    binary_string[i] = '\0';
}

void set_binary_string_opcode_representation(int opcode_number, char *binary_string) {
    int i;

    if (binary_string == NULL) {
        print_internal_error(ERROR_CODE_38, "");
        exit(EXIT_FAILURE);
    }

    // Mask the opcode number to fit within the size defined by OPCODE_SIZE
    int binary_value = opcode_number & ((1 << OPCODE_SIZE) - 1);

    // Convert the binary value to a string of '0's and '1's
    for (i = 0; i < OPCODE_SIZE; i++) {
        // Check each bit from the most significant to the least significant
        if (binary_value & (1 << (OPCODE_SIZE - 1 - i))) {
            binary_string[i] = '1'; // Set '1' if the bit is set
        } else {
            binary_string[i] = '0'; // Set '0' if the bit is not set
        }
    }
}


void fill_second_part_binary_opcode(InstructionLine *instruction_line, char *binary_string, SymbolTable *symbol_table) {
    int operand_number;          // Number of operands in the instruction
    Operand *src_operand;        // Pointer to the source operand
    Operand *dst_operand;        // Pointer to the destination operand
    int ic;                      // Instruction counter (starting address of the instruction)
    char file_name[100];

    // Initialize local variables from the instruction line
    operand_number = instruction_line->command->operand_number;
    src_operand = instruction_line->command->src_operand;
    dst_operand = instruction_line->command->dst_operand;
    strcpy(file_name, instruction_line->file_name);
    ic = instruction_line->starting_address;

    // Process the binary representation based on the number of operands
    switch (operand_number) {
        case 2: // Instruction has both source and destination operands
            // Handle the source operand
            switch (src_operand->classification_type) {
                case IMMEDIATE:
                    fill_immediate_binary(src_operand, binary_string, SRC_OPERAND_NUMBER);
                    break;
                case DIRECT:
                    fill_direct_binary(src_operand, binary_string, SRC_OPERAND_NUMBER, ic, symbol_table, file_name);
                    break;
                case DIRECT_REGISTER:
                case INDIRECT_REGISTER:
                    fill_register_binary(src_operand, NULL, binary_string, SRC_OPERAND_NUMBER);
                    break;
                case METHOD_UNKNOWN:
                    break;
            }
            // Handle the destination operand
            switch (dst_operand->classification_type) {
                case IMMEDIATE:
                    fill_immediate_binary(dst_operand, binary_string, DST_OPERAND_NUMBER);
                    break;
                case DIRECT:
                    fill_direct_binary(dst_operand, binary_string, DST_OPERAND_NUMBER, ic, symbol_table, file_name);
                    break;
                case DIRECT_REGISTER:
                case INDIRECT_REGISTER:
                    if (is_classification_type_register(src_operand->classification_type)) {
                        // If both source and destination operands are registers, combine them in the same binary word
                        fill_register_binary(NULL, dst_operand, binary_string, 1);
                    } else {
                        fill_register_binary(NULL, dst_operand, binary_string, DST_OPERAND_NUMBER);
                    }
                    break;
                case METHOD_UNKNOWN:
                    break;
            }
            break;
        case 1: // Instruction has only a destination operand
            switch (dst_operand->classification_type) {
                case IMMEDIATE:
                    fill_immediate_binary(dst_operand, binary_string, 1);
                    break;
                case DIRECT:
                    fill_direct_binary(dst_operand, binary_string, 1, ic, symbol_table, file_name);
                    break;
                case DIRECT_REGISTER:
                case INDIRECT_REGISTER:
                    fill_register_binary(NULL, dst_operand, binary_string, 1);
                    break;
                case METHOD_UNKNOWN:
                    break;
            }
            break;
        case 0: // Instruction has no operands
            break;
        default:
            print_internal_error(ERROR_CODE_23, int_to_string(operand_number));
            break;
    }
}

void fill_immediate_binary(Operand *operand, char *binary_string, int binary_word_number) {
    int int_number_to_binary;  // Integer value to be converted to binary

    if (operand == NULL) {
        print_internal_error(ERROR_CODE_24, " Operand is empty");
        return;
    }

    // Convert the operand's immediate value from string to integer
    int_number_to_binary = char_to_int(operand->value);

    // Convert the integer to binary and store it in the binary string
    int_to_binary_string(int_number_to_binary, binary_string, binary_word_number * BINARY_LINE_LENGTH,
                         INT_TO_BINARY_NUMBER_OF_BITS);

    // Set the ARE (Absolute, Relocatable, External) bit for the binary word
    set_binary_string_ARE_representation(binary_string, binary_word_number + 1, 'a');
}


void fill_direct_binary(Operand *operand, char *binary_string, int binary_word_number, int ic, SymbolTable *symbol_table, char* file_name) {
    Symbol *symbol;                // Pointer to the symbol associated with the operand
    int int_number_to_binary;       // Integer value representing the symbol's address

    if (operand == NULL) {
        print_internal_error(ERROR_CODE_24, " Operand is empty");
        return;
    }

    // Find the symbol by name in the symbol table
    symbol = find_symbol_by_name(symbol_table, operand->value);
    if (symbol == NULL) {
        print_internal_error(ERROR_CODE_51, " Symbol is empty");
        return;
    }
    // Convert symbol address to binary and update the binary string
    int_number_to_binary = symbol->address;
    int_to_binary_string(int_number_to_binary, binary_string, binary_word_number * BINARY_LINE_LENGTH, 12);

    if (symbol->is_extern) { // External symbol
        if (binary_word_number == 1) {
            ic += 1;
        } else if(binary_word_number == 2){
            ic += 2;
        }
        add_extern_to_externals_file(symbol->name, file_name, ic);
        set_binary_string_ARE_representation(binary_string, binary_word_number + 1, 'e');
    } else { // Internal symbol
        set_binary_string_ARE_representation(binary_string,
                                             binary_word_number + 1, 'r');
    }
}

void fill_register_binary(Operand *src_operand, Operand *dst_operand, char *binary_string, int binary_word_number) {
    char *src_register_value;  // Value of the source register
    char *dst_register_value;  // Value of the destination register

    if (src_operand != NULL) {
        src_register_value = src_operand->value;
        register_to_binary_string(src_register_value, SRC_OPERAND_NUMBER, binary_string,
                                  BINARY_LINE_LENGTH * binary_word_number);
    }

    if (dst_operand != NULL) {
        dst_register_value = dst_operand->value;
        register_to_binary_string(dst_register_value, DST_OPERAND_NUMBER, binary_string,
                                  BINARY_LINE_LENGTH * binary_word_number);
    }

    // Set the ARE (Absolute, Relocatable, External) bit for the binary word
    set_binary_string_ARE_representation(binary_string, binary_word_number + 1, 'a');
}

void register_to_binary_string(char *register_value, int operand_number, char *binary_string, int offset) {
    int register_number = char_to_int(register_value);

    // Convert register number to binary and place in the correct bit positions
    if (operand_number == 2) { // Destination register (bits 9-11)
        int_to_binary_string(register_number, binary_string, offset + DST_REGISTER_OFFSET,
                             REGISTER_TO_BINARY_NUMBER_OF_BITS);
    } else if (operand_number == 1) { // Source register (bits 6-8)
        int_to_binary_string(register_number, binary_string, offset + SRC_REGISTER_OFFSET,
                             REGISTER_TO_BINARY_NUMBER_OF_BITS);
    }
}


void fill_binary_directive(InstructionLine *instruction_line, char *binary_string) {
    int i;
    int number_value;
    Directive *directive;
    directive = instruction_line->directive;

    // Process DATA directive
    if (is_directive_data(directive)) {
        if (directive->value == NULL) {
            print_internal_error(ERROR_CODE_33, "");
            return;
        }
        for (i = 0; i < directive->data_values_count; ++i) {
            if (directive->value[i] == NULL) {
                print_internal_error(ERROR_CODE_34, "");
                return;
            }
            number_value = char_to_int(directive->value[i]);
            int_to_binary_string(number_value, binary_string, i * BINARY_LINE_LENGTH,
                                 BINARY_LINE_LENGTH);
        }
    }
    // Process STRING directive
    else if (is_directive_string(directive)) {
        if (directive->value == NULL) {
            print_internal_error(ERROR_CODE_33, "");
            return;
        }
        if (directive->value[0] == NULL) {
            print_internal_error(ERROR_CODE_34, "");
            return;
        }
        for (i = 0; i < strlen(directive->value[0]); ++i) {
            char_to_binary_string(directive->value[0][i], binary_string,
                                  i * BINARY_LINE_LENGTH, BINARY_LINE_LENGTH);
        }
    }
}

