#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "output_test.h"
#include "../include/assembler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "output_test.h"
#include "../include/assembler.h"

/**
 * Runs a series of output tests to compare generated files with expected results.
 * The function tests assembly output against predefined correct output files.
 *
 * @return 1 if all tests pass, 0 if any test fails.
 */
int output_test() {
    int i;
    char input_code_fname[100] = "Tests/Test_global/ps.as";
    char output_code_fname[100];
    char output_correct_fname[100];
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;

    printf("Starting output tests\n");


    // Define the test cases
    int number_of_tests = 2;
    char *test[] = {"../../Tests/Test_global", "../../Tests/Test_entry"};
    printf("input is %s, output is %s\n",input_code_fname, output_code_fname);
    // Iterate over each test case
    for (i = 0; i < number_of_tests; ++i) {
        // Run the assembler on the test file
        run_assembler_on_file(assembly_lines_array, symbol_table, input_code_fname, i);

        // Compare object files
        strcpy(output_correct_fname, test[i]);
        strcat(output_correct_fname, "/ps.ob");
        add_number_to_string(output_code_fname, OBJECTS_FILE_NAME, i);
        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
            printf("The output test Failed! \nFor input file %s and output object file %s\n",
                   input_code_fname, output_code_fname);
            return 0;
        }

        // Compare extern files
        strcpy(output_correct_fname, test[i]);
        strcat(output_correct_fname, "/ps.ext");
        add_number_to_string(output_code_fname, EXTERNALS_FILE_NAME, i);
        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
            printf("The output test Failed! \nFor input file %s and output extern file %s\n",
                   input_code_fname, output_code_fname);
            return 0;
        }

        // Compare entry files
        strcpy(output_correct_fname, test[i]);
        strcat(output_correct_fname, "/ps.ent");
        add_number_to_string(output_code_fname, ENTRIES_FILE_NAME, i);
        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
            printf("The output test Failed! \nFor input file %s and output entry file %s\n",
                   input_code_fname, output_code_fname);
            return 0;
        }
    }

    return 1; // All tests passed
}
