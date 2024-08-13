#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "output_test.h"
#include "../include/assembler.h"

int output_test(char *input_code_fname, char *output_correct_fname){
    char *output_code_fname;
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;
    printf("Starting output tests\n");
    run_assembler_on_file(assembly_lines_array,symbol_table, input_code_fname, 0);

    add_number_to_string(output_code_fname, input_code_fname, sizeof(a), 0);

    if (!open_two_files_and_compare(add_number_to_string(), test_output_file_name)){  //0 if identical and 1 if not
        printf("The output test Failed! \nFor input file %s and output file %s\n",
               test_input_file_name, test_output_file_name);
        return 0;
    }
    if (!open_two_files_and_compare(, test_output_file_name)){  //0 if identical and 1 if not
        printf("The output test Failed! \nFor input file %s and output file %s\n",
               test_input_file_name, test_output_file_name);
        return 0;
    }
    if (!open_two_files_and_compare(, test_output_file_name)){  //0 if identical and 1 if not
        printf("The output test Failed! \nFor input file %s and output file %s\n",
               test_input_file_name, test_output_file_name);
        return 0;
    }
    return 1;
}
