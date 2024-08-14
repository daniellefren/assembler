#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../include/second_run.h"
#include "../include/files_handler.h"
#include "../include/errors.h"


void start_second_run(LinesArray *assembly_lines_array, int file_number, SymbolTable *symbol_table){
    int i;
    InstructionLine *p_line;

    printf("Starting Second run \n");
    printf("The number of lines in the struct is %d \n \n", assembly_lines_array->number_of_line);
    printf("Starting to print The lines Binary: \n");

    // Iterate over each line in the assembly lines array
    for (i = 0; i < assembly_lines_array->number_of_line; ++i){
        p_line = assembly_lines_array->lines[i];
        printf("\n****Line number %d Address %d *****  \n\n", i, p_line->starting_address);
        allocate_binary_instruction(p_line, p_line->binary_line_count, BINARY_LINE_LENGTH);
        fill_instruction_line_binary(p_line, symbol_table);
        print_instruction_line(p_line);
    }

    create_ob_file(assembly_lines_array, file_number);
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

    printf("The number of lines in the binary is: %d \n", binary_line_count);
    // Initialize the binary instruction array with zeros
    fill_the_binary_with_zero(binary_instruction_p, BINARY_LINE_LENGTH * binary_line_count);
    if (is_instruction_line_opcode(instruction_line)){
        fill_first_part_binary_opcode(instruction_line, binary_instruction_p);
        printf("First part binary -                                      %s \n\n", binary_instruction_p);
        if (binary_line_count > 1){
            fill_second_part_binary_opcode(instruction_line, binary_instruction_p, symbol_table);
            printf("Second part binary -                                     %s \n", binary_instruction_p);
        }
    }
    else if (is_instruction_line_directive(instruction_line)){
        fill_binary_directive(instruction_line, binary_instruction_p);
    }
    printf("***The binary representation of the line is %s \n", instruction_line->binary_instruction);
}

void fill_first_part_binary_opcode(InstructionLine *instruction_line, char *binary_string) {
    int binary_string_number = 1; //first part binary
    int first_operand_classification = -1; // Set to -1 to indicate no second operand if only one operand is present
    int second_operand_classification = -1;

    // Convert the opcode value to its binary representation and store it in binary_string
    set_binary_string_opcode_representation(instruction_line->command->opcode_command_type, binary_string);

    // Set the ARE (Addressing, Register, or External) representation in the binary string
    set_binary_string_ARE_representation(binary_string, binary_string_number, 'a');
    printf("The binary string with ARE is:                           %s \n", binary_string);
    /* TODO wait for dani to swap src and dst operand when only 1 operand
    if (instruction_line->command->operand_number <= 0) {
        return; // No operands present, exit function

    } else if (instruction_line->command->operand_number > 1){ //2 operand mean src and dst
        first_operand_classification = instruction_line->command->src_operand->classification_type;

    }
    second_operand_classification = instruction_line->command->dst_operand->classification_type; //1 operand can be just dst
    */
    if (instruction_line->command->operand_number > 0) {
        first_operand_classification = instruction_line->command->src_operand->classification_type;
    } else {
        return; // No operands present, exit function
    }
    if (instruction_line->command->operand_number > 1) {
        second_operand_classification = instruction_line->command->dst_operand->classification_type;
    }

    // Convert operand classifications to their binary representation and update the binary string
    set_binary_string_operand_representation(first_operand_classification,
                                             second_operand_classification, binary_string);
    printf("The binary string with operand is:                       %s \n", binary_string);
}



void set_binary_string_operand_representation(int first_operand_classification_type,
                                              int second_operand_classification_type, char *binary_string) {
    int offset;
    int operand_binary_classification_size = 4;
    int first_classification_offset = 3; //The offset where the first classification start at the binary
    int second_classification_offset = 7; //The offset where the second classification start at the binary

    printf("First classification type is %d \n", first_operand_classification_type);
    printf("second classification type is %d \n", second_operand_classification_type);
    if (is_operand_classification_type_valid(first_operand_classification_type)) {
        //bit 5 = method 3, bit 6 method 2, bit 7 method 1, bit 8 method 0
        offset = first_classification_offset + operand_binary_classification_size - first_operand_classification_type;
        binary_string[offset] = '1';
    }
    if (is_operand_classification_type_valid(second_operand_classification_type)) {
        //bit 9 = method 3, bit 10 method 2, bit 11 method 1, bit 12 method 0
        offset = second_classification_offset + operand_binary_classification_size - second_operand_classification_type;
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
    printf("The binary string (filled with zero) is: %s \n", binary_string);
}

void set_binary_string_opcode_representation(int opcode_number, char *binary_string) {
    int i;
    printf("The opcode is %d and the binary string is %s\n", opcode_number, binary_string);

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
    printf("The opcode binary string after opcode representation is: %s \n", binary_string);
}


void fill_second_part_binary_opcode(InstructionLine *instruction_line, char *binary_string, SymbolTable *symbol_table) {
    int operand_number;
    Operand *src_operand;
    Operand *dst_operand;

    operand_number = instruction_line->command->operand_number;
    src_operand = instruction_line->command->src_operand;
    dst_operand = instruction_line->command->dst_operand;
    // Process the binary representation based on the number of operands
    switch (operand_number) {
        case 2:
            // Process and append the binary representation for both source and destination operands
            fill_operand_binary(src_operand, dst_operand, binary_string,
                                SRC_OPERAND_NUMBER, instruction_line->starting_address, instruction_line->file_number, symbol_table);
            fill_operand_binary(dst_operand, NULL, binary_string,
                                DST_OPERAND_NUMBER, instruction_line->starting_address, instruction_line->file_number, symbol_table);
            break;
        case 1:
            fill_operand_binary(src_operand, NULL, binary_string,
                                SRC_OPERAND_NUMBER, instruction_line->starting_address, instruction_line->file_number, symbol_table);
            /* TODO - use after dani swap src and dst
            fill_operand_binary(dst_operand, NULL, binary_string,
                                SRC_OPERAND_NUMBER, instruction_line->starting_address, instruction_line->file_number, symbol_table);
            */
            break;
        case 0:
            break;
        default:
            print_internal_error(ERROR_CODE_23, int_to_string(operand_number));
            break;
    }
}

void fill_operand_binary(Operand *operand, Operand *second_operand, char *binary_string, int operand_number, int ic, int file_number, SymbolTable *symbol_table) { //TODO - Danielle added ic
    int int_number_to_binary;
    char *register_value;
    Symbol *symbol;

    // Handle the different operand classification types
    switch (operand->classification_type) {
        case IMMEDIATE:
            // Convert immediate value to binary and update the binary string
            int_number_to_binary = char_to_int(operand->value);
            int_to_binary_string(int_number_to_binary, binary_string,
                                 operand_number * BINARY_LINE_LENGTH, 12);
            set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
            break;
        case DIRECT:
            symbol = find_symbol_by_name(symbol_table, operand->value);
            // Convert symbol address to binary and update the binary string
            int_number_to_binary = operand->symbol->address;
            int_to_binary_string(int_number_to_binary, binary_string,
                                 operand_number * BINARY_LINE_LENGTH, 12);
            if (symbol->is_extern) { // external
                if(operand_number == 2){
                    ic+=1;
                }
                add_extern_to_externals_file(symbol->name, file_number, ic); // TODO - Add 0 in the beginning ov ic
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'e');
            } else { // internal
                set_binary_string_ARE_representation(binary_string, operand_number + 1, 'r');
            }
            break;
        case INDIRECT_REGISTER:
        case DIRECT_REGISTER:
            // Handle register operand TODO - need to swap here afte dani swap src and dst
            register_value = operand->value;
            if (operand_number == 1) { // Source operand
                if (second_operand != NULL) {
                    if ((second_operand->classification_type == INDIRECT_REGISTER) ||
                        (second_operand->classification_type == DIRECT_REGISTER)) { // Both operands are registers
                        register_to_binary_string(register_value, operand_number, binary_string,
                                                  BINARY_LINE_LENGTH);
                        register_value = second_operand->value;
                        register_to_binary_string(register_value, operand_number + 1, binary_string,
                                                  BINARY_LINE_LENGTH);
                        set_binary_string_ARE_representation(binary_string,
                                                             operand_number + 1,'a');
                    } else { // Source operand with destination as direct or immediate
                        register_to_binary_string(register_value, operand_number, binary_string,
                                                  BINARY_LINE_LENGTH);
                        set_binary_string_ARE_representation(binary_string,
                                                             operand_number + 1,'a');
                    }
                }
            } else { // Destination operand
                int a_offset = (BINARY_LINE_LENGTH * operand_number) + 12;
                if (binary_string[a_offset] == '0') { // The source operand wasn't a register
                    register_to_binary_string(register_value, operand_number, binary_string, BINARY_LINE_LENGTH);
                    set_binary_string_ARE_representation(binary_string, operand_number + 1, 'a');
                }
            }
            break;
        case METHOD_UNKNOWN:
            // No action for unknown method
            break;
    }
}

void register_to_binary_string(char *register_value, int operand_number, char *binary_string, int offset) {
    int register_number = char_to_int(register_value);

    // Convert register number to binary and place in the correct bit positions
    if (operand_number == 2) { // Destination register (bits 9-11)
        int_to_binary_string(register_number, binary_string, offset + DST_REGISTER_OFFSET, 3);
    } else if (operand_number == 1) { // Source register (bits 6-8)
        int_to_binary_string(register_number, binary_string, offset + SRC_REGISTER_OFFSET, 3);
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

