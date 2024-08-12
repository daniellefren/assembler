#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "output_test.h"
#include "../../src/assembler.c"

int output_test(char *test_input_file_name, char *test_output_file_name){
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;

    run_assembler_on_file(assembly_lines_array,symbol_table, test_input_file_name,1);
    if (strcmp(test_input_file_name, test_output_file_name)){
        return 1;
    }
    return 0;
}