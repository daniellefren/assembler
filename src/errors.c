#include <stdio.h>
#include "../include/errors.h"

Error errors[] = {
        {ERROR_CODE_0,  "No Error"},

        // Readind assembly lines
        {ERROR_CODE_1,  "Macro is a known assembly keyword"},
        {ERROR_CODE_2,  "Symbol is not valid -- a space between the symbol and the ':'"},
        {ERROR_CODE_3,  "Symbol is not valid -- is a known assembly word"},
        {ERROR_CODE_4,  "Symbol is not valid -- matches an existing macro name"},
        {ERROR_CODE_5,  "Symbol is not valid -- too long"},
        {ERROR_CODE_6,  "Symbol is not valid -- should start with a letter"},
        {ERROR_CODE_7,  "Symbol is not valid -- should only contain with letters and numbers"},
        {ERROR_CODE_8,  "Program stopped running because of incorrect given assembly code"},


        // Memory allocation failed
        {ERROR_CODE_9,  "Unable to allocate memory"},
        {ERROR_CODE_10,  "Unable to allocate memory for macro table"},
        {ERROR_CODE_11,  "Unable to allocate memory for symbol"},
        {ERROR_CODE_12,  "Unable to allocate memory for directive"},
        {ERROR_CODE_13,  "Unable to allocate memory for directory values"},
        {ERROR_CODE_14,  "Unable to allocate memory for Symbol array"},
        {ERROR_CODE_15,  "Unable to allocate memory for lines array"},
        {ERROR_CODE_16,  "Unable to allocate memory for instruction line"},
        {ERROR_CODE_17,  "Unable to allocate memory for line content in Instruction line"},
        {ERROR_CODE_18,  "Unable to allocate memory for directive"},
        {ERROR_CODE_19,  "Unable to allocate memory for command name in Command"},
        {ERROR_CODE_20,  "Unable to allocate memory for Operand"},
        {ERROR_CODE_21,  "Unable to allocate memory for macro name in Macro"},
        {ERROR_CODE_22,  "Unable to allocate memory for binary instruction in Instruction line"},

        //Internal errors
        {ERROR_CODE_23,  "Invalid number of operands"},
        {ERROR_CODE_24,  "Not valid operand type"},
        {ERROR_CODE_25,  "Not valid classification type"},
        {ERROR_CODE_26,  "Warning: Reached maximum number of macro names"},
        {ERROR_CODE_27,  "SymbolTable pointer is NULL"},
        {ERROR_CODE_28,  "Lines array is NULL"},
        {ERROR_CODE_29,  "Line number in lines array is NULL"},
        {ERROR_CODE_30,  "Line number in lines array is NULL"},
        {ERROR_CODE_31,  "Error: Opcode not found in the instruction set"},
        {ERROR_CODE_32,  "Failed to read directive type"},
        {ERROR_CODE_33,  "Trying to access a directive->value but the value is NULL"},
        {ERROR_CODE_34,  "Trying to access a directive->value[i] but the value is NULL"},
        {ERROR_CODE_35,  "0 lines of binary for the instruction line"},
        {ERROR_CODE_36,  "Character is not 'a', 'r', or 'e'"},
        {ERROR_CODE_37,  "trying to fill the binary with zero but the length <= 0"},
        {ERROR_CODE_38,  "The binary string is null"},
        {ERROR_CODE_39,  "Error finding symbol"},
        {ERROR_CODE_40,  "insufficient space to append both strings"},
        {ERROR_CODE_41,  "there are no values in the directive"},
        {ERROR_CODE_42,  "Invalid input parameters"},
        {ERROR_CODE_43,  "Number of bits must be a multiple of 3"},
        {ERROR_CODE_44,  "Invalid binary string"},
        {ERROR_CODE_45,  "Invalid binary string buffer"},
        {ERROR_CODE_46,  "LinesArray pointer is NULL"},
        {ERROR_CODE_47,  "Unknown directive"},


        // File related errors
        {ERROR_CODE_48,  "Could not open file for writing"},
        {ERROR_CODE_49,  "Could not open file for writing macros"},

};

void print_internal_error(int error_code, char* data) {
    if(data){
        fprintf(stderr, "~~ERROR: ID:%d~~ | %s - %s\n", error_code, errors[error_code].error_msg, data);
    }
    else{
        fprintf(stderr, "~~ERROR: ID:%d~~ | %s\n", error_code, errors[error_code].error_msg);
    }
}
