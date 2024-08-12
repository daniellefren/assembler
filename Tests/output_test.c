#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "output_test.h"
#include "../include/assembler.h"

int output_test(char *test_input_file_name, char *test_output_file_name){
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;
    printf("Starting output tests\n");
    run_assembler_on_file(assembly_lines_array,symbol_table, "Tests/ExampleFiles/ps.as",1);
    /*
    if (strcmp(test_input_file_name, test_output_file_name)){
        return 1;
    }*/
    return 0;

}
