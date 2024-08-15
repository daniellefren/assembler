
#ifndef ASSEMBLER_OUTPUT_TEST_H
#define ASSEMBLER_OUTPUT_TEST_H


int output_test();
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
int compare_output_files(int file_number, char* test_file_name, char* output_code_fname, char* input_code_fname, char* output_correct_fname, char* correct_file_name_ending);

#endif //ASSEMBLER_OUTPUT_TEST_H
