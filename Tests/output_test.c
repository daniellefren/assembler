#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>  // for getcwd

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
    char input_code_fname[100];
    char output_code_fname[100];
    char output_correct_fname[100];
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;

    printf("Starting output tests\n");
    print_current_directory();


    // Define the test cases
    int number_of_tests = 2;
    char *test[] = {"/Users/daniellefrenklakh/Desktop/University/C/assembler/Tests/Test_global"};
    // Iterate over each test case
    for (i = 0; i < number_of_tests; ++i) {
        strcpy(input_code_fname, test[i]);
        strcat(input_code_fname, "/ps.as");

        add_number_to_string(output_code_fname, sizeof(output_code_fname), OBJECTS_FILE_NAME, i+1);

        printf("input is %s, output is %s\n",input_code_fname, output_code_fname);
        // Run the assembler on the test file
        run_assembler_on_file(assembly_lines_array, symbol_table, input_code_fname, i);

        // Compare object files
        strcpy(output_correct_fname, test[i]);
        strcat(output_correct_fname, "/ps.ob");
        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
            printf("The output test Failed! \nFor input file %s and output object file %s\n",
                   input_code_fname, output_code_fname);
            return 0;
        }

        // Compare extern files
//        strcpy(output_correct_fname, test[i]);
//        strcat(output_correct_fname, "/ps.ext");
//        add_number_to_string(output_code_fname, EXTERNALS_FILE_NAME, i+1);
//        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
//            printf("The output test Failed! \nFor input file %s and output extern file %s\n",
//                   input_code_fname, output_code_fname);
//            return 0;
//        }
//
//        // Compare entry files
//        strcpy(output_correct_fname, test[i]);
//        strcat(output_correct_fname, "/ps.ent");
//        add_number_to_string(output_code_fname, ENTRIES_FILE_NAME, i+1);
//        if (!open_two_files_and_compare(output_correct_fname, output_code_fname)) {
//            printf("The output test Failed! \nFor input file %s and output entry file %s\n",
//                   input_code_fname, output_code_fname);
//            return 0;
//        }
    }

    return 1; // All tests passed
}


void print_current_directory(){
    char *cwd;
    size_t size = 1024;  // Initial buffer size

    // Allocate memory for the buffer
    cwd = (char *)malloc(size * sizeof(char));
    if (cwd == NULL) {
        perror("Unable to allocate buffer");
        return;
    }

    // Get the current working directory
    if (getcwd(cwd, size) == NULL) {
        perror("getcwd error");
        free(cwd);
        return;
    }

    // Print the current working directory
    printf("Current working directory: %s\n", cwd);

    // Free the allocated buffer
    free(cwd);
}