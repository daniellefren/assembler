#include <stdio.h>
#include <string.h>


#include "output_test.h"
#include "../include/assembler.h"



/**
 * Runs a series of output tests to compare generated files with expected results.
 * The function tests assembly output against predefined correct output files.
 *
 * @return 1 if all tests pass, 0 if any test fails.
 */
int output_test(void) {

    int i;
    int number_of_tests;
    char input_code_fname[100];
    char output_code_fname[100];
    char output_code_fname_with_directive[100];
    char output_correct_fname[100];
    int is_success;
    char *test[5];

    printf("Starting tests\n");

    /* Define the test cases */

    test[0] = "./Tests/Test_global/test1.as";
    test[1] = "./Tests/Test_entries/test_entry.as";
    test[2] = "./Tests/Test_externals/test_externals.as";
    test[3] = "./Tests/Test_macros/test_macro.as";
    test[4] = NULL;

    number_of_tests = count_strings(test);
    is_success = 1;
    /* Iterate over each test case*/
    for (i = 0; i < number_of_tests; i++) {
        printf("\nStarting to create test number %d\n\n", i+1);
        /*Get input file name */
        strcpy(input_code_fname, test[i]);
        strcpy(output_correct_fname, input_code_fname);

        /* Run the assembler on the test file */
        run_assembler_on_file(input_code_fname, i + 1);
        get_output_filename(output_code_fname, output_code_fname_with_directive, OBJECT_FILE_EXTENSION, get_filename(test[i]));

        printf("\nStarting comparing test number %d\n\n", i+1);
        printf("Compare object file\n");
        is_success &= compare_output_files(output_code_fname_with_directive, input_code_fname, output_correct_fname, OBJECT_FILE_EXTENSION);

        printf("\nCompare extern file\n");
        is_success &= compare_output_files(output_code_fname_with_directive, input_code_fname, output_correct_fname, EXTERNALS_FILE_EXTENSION);

        printf("\nCompare entry file\n");
        is_success &= compare_output_files( output_code_fname_with_directive, input_code_fname, output_correct_fname, ENTRIES_FILE_EXTENSION);

        if(is_success){
            printf("All tests passed for test %d\n", i+1);
        }

    }
    return is_success;
}

int compare_output_files(char* output_code_fname, char* input_code_fname, char* output_correct_fname, const char* file_extension){
    replace_extension(output_code_fname, file_extension);
    replace_extension(output_correct_fname, file_extension);

    /* Compare object files */
    if (open_two_files_and_compare(output_correct_fname, output_code_fname) == 0) {
        printf("The output test Failed! \nFor input file %s and output object file %s\n",
               input_code_fname, output_code_fname);
        return 0;
    }
    return 1;
}

int count_strings(char *strings[]) {
    int count;
    count = 0;
    while (strings[count] != NULL) {
        count++;
    }
    return count;
}
