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
int output_test() {

    int i;
    int number_of_tests;
    char input_code_fname[100];
    char output_code_fname[100];
    char output_code_fname_with_directive[100];
    char output_correct_fname[100];
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;

    printf("Starting output tests\n");

    // Define the test cases

    char *test[] = {"./Tests/Test_global/test1.as", "./Tests/Test_entries/test_entry.as", "./Tests/Test_externals/test_externals.as", NULL};

    number_of_tests = count_strings(test);
    printf("number_of_tests %d", number_of_tests);
    // Iterate over each test case
    for (i = 0; i < number_of_tests; i++) {
        //Get input file name
        strcpy(input_code_fname, test[i]);
        strcpy(output_correct_fname, input_code_fname);

        printf("input_code_fname %s\n", input_code_fname);

        // Run the assembler on the test file
        run_assembler_on_file(assembly_lines_array, symbol_table, input_code_fname, i + 1);
        printf("\nStarting comparing test number %d\n\n", i+1);

        printf("Compare object file\n");
        get_output_filename(output_code_fname, output_code_fname_with_directive, OBJECT_FILE_EXTENSION, get_filename(test[i]));

        replace_extension(output_code_fname_with_directive, OBJECT_FILE_EXTENSION);
        replace_extension(output_correct_fname, OBJECT_FILE_EXTENSION);

        compare_output_files(i+1, test[i], output_code_fname_with_directive, input_code_fname, output_correct_fname);

        // TODO - compare only if exists
        printf("\nCompare extern file\n");
        replace_extension(output_code_fname_with_directive, EXTERNALS_FILE_EXTENSION);
        replace_extension(output_correct_fname, EXTERNALS_FILE_EXTENSION);

        compare_output_files(i+1, test[i], output_code_fname_with_directive, input_code_fname, output_correct_fname);

        printf("\nCompare entry file\n");
        replace_extension(output_code_fname_with_directive, ENTRIES_FILE_EXTENSION);
        replace_extension(output_correct_fname, ENTRIES_FILE_EXTENSION);

        compare_output_files(i+1, test[i], output_code_fname_with_directive, input_code_fname, output_correct_fname);

    }
    return 1; // All tests passed
}

int compare_output_files(int file_number, char* test_file_name, char* output_code_fname, char* input_code_fname, char* output_correct_fname){
    //get output filename
    printf("input is %s, output is %s\n", input_code_fname, output_code_fname);

    // Compare object files
    if (open_two_files_and_compare(output_correct_fname, output_code_fname) == 0) {
        printf("The output test Failed! \nFor input file %s and output object file %s\n",
               input_code_fname, output_code_fname);
        return 0;
    }
    return 1;
}

/**
 * @brief Counts the number of strings in a null-terminated array of strings.
 *
 * This function iterates through the array of strings until it encounters a NULL pointer,
 * counting the number of valid strings in the array.
 *
 * @param strings The null-terminated array of strings.
 * @return The number of strings in the array.
 */
int count_strings(char *strings[]) {
    int count;
    count = 0;
    while (strings[count] != NULL) {
        count++;
    }
    return count;
}