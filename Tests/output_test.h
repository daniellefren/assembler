
#ifndef ASSEMBLER_OUTPUT_TEST_H
#define ASSEMBLER_OUTPUT_TEST_H

/**
 * Run the tests
 * @return 1 if all tests passed, else 0
 */
int output_test(void);
/**
 * @brief Counts the number of strings in a null-terminated array of strings.
 *
 * This function iterates through the array of strings until it encounters a NULL pointer,
 * counting the number of valid strings in the array.
 *
 * @param strings The null-terminated array of strings.
 * @return The number of strings in the array.
 */
int count_strings(char *strings[]);

/**
 * Compares between files in order to check if the output is identical to the correct output
 * @param output_code_fname The file name of the output file from the assembler run
 * @param input_code_fname The file name of the input assembler code
 * @param output_correct_fname The file name of the correct output for the assembler code
 * @param file_extension The file extension from the file the current test will check
 * @return The number of strings in the array.
 * @return
 */
int compare_output_files(char* output_code_fname, char* input_code_fname, char* output_correct_fname, const char* file_extension);

#endif
