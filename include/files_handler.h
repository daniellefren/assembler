
#ifndef ASSEMBLER_FILES_HANDLER_H
#define ASSEMBLER_FILES_HANDLER_H
#include "../include/utils.h"


/**
 * Create an output directory if it does not already exist.
 */
void add_output_directory(void);


/**
 * Add an external symbol to the externals file.
 *
 * @param symbol - Pointer to the Symbol struct representing the external symbol.
 * @param file_number - The file number used to generate the externals file name.
 * @param ic - Pointer to the instruction counter (IC) which indicates the address of the symbol.
 */
void add_extern_to_externals_file(char *symbol_name, int file_number, int ic);

/**
 * Add an entry symbol to the entries file.
 *
 * @param symbol_name - The name of the symbol to add as an entry.
 * @param file_number - The file number used to generate the entries file name.
 * @param symbol_address - The memory address of the entry symbol.
 */
void add_entry_to_entries_file(char *symbol, int file_number, int symbol_address);

/**
 * Write a line of text to a file.
 *
 * @param line - The line of text to write to the file.
 * @param new_file_name - The name of the file to write the line to.
 * @return int - Returns 0 to indicate success.
 */
int write_line_to_file(char *line, char* new_file_name);

/**
 * Write the expanded macros from the MacroTable to a file.
 *
 * @param macro_table - Pointer to the MacroTable containing the expanded macros.
 * @param new_file_name - The name of the file to write the expanded macros to.
 */
void write_expanded_macros_to_file(MacroTable *macro_table, char* new_file_name);

/**
 * Opens a new file for writing object code.
 *
 * This function attempts to open a file with the specified name for writing. If the file
 * cannot be opened, the function prints an error message and terminates the program.
 *
 * @param ob_file_name The name of the file to open. It should be a valid string containing
 *                     the name of the file to be created or overwritten.
 * @return A FILE pointer to the opened file. If the file cannot be opened, the program
 *         exits with a failure status.
 */
FILE* open_ob_file(char *ob_file_name);

/**
 * Creates an object file based on the lines of assembly code provided in `linesArray`.
 * The function generates the file name, opens the file, and then writes the commands and directives
 * from the `linesArray` to the object file. The file includes a header with the count of commands
 * and directives.
 *
 * @param linesArray A pointer to the `LinesArray` structure containing the lines of assembly code.
 *                   This structure holds the instruction counter (IC) and directive counter (DC)
 *                   along with the actual lines of code.
 * @param file_number An integer representing the file number to be appended to the object file name.
 */
void create_ob_file(LinesArray *linesArray, int file_number);

/**
 * Writes the first line to the object file.
 *
 * This function writes the number of commands and directives to the first line
 * of the given object file. If the file pointer is NULL, the function will print
 * an error message and terminate the program.
 *
 * @param number_of_command The number of commands to write to the file.
 * @param number_of_directive The number of directives to write to the file.
 * @param object_file A pointer to the FILE where the data will be written.
 *                    It should be a valid, open file in write mode.
 */
void add_first_line_to_ob_file(int number_of_command, int number_of_directive, FILE *object_file);

/**
 * Adds all command lines from the `lines_array` to the object file.
 * The function iterates over each line in the `lines_array`, checks if it is a command,
 * and if so, writes the command details (starting address, binary content, etc.) to the object file.
 *
 * @param lines_array A pointer to the `LinesArray` structure that contains all lines of code to be processed.
 * @param object_file A pointer to the open object file where the command lines will be written.
 */
void add_all_command_lines_to_ob_file(LinesArray *lines_array, FILE *object_file);

/**
 * Adds all directive lines from the `lines_array` to the object file.
 * The function iterates over each line in the `lines_array`, checks if it is a data directive,
 * and if so, writes the directive details (starting address, binary content, etc.) to the object file.
 *
 * @param lines_array A pointer to the `LinesArray` structure that contains all lines of code to be processed.
 * @param object_file A pointer to the open object file where the directive lines will be written.
 */
void add_all_directive_lines_to_ob_file(LinesArray *lines_array, FILE *object_file);

/**
 * Adds the binary command line to the object file in octal format. This function converts the
 * binary representation of the command line into an octal string and writes it to the object file
 * along with the instruction address.
 *
 * @param instructionLine A pointer to the `InstructionLine` structure that represents the command
 *                        to be written to the object file.
 * @param object_file A pointer to the `FILE` object representing the object file where the command
 *                    line will be written.
 */
void add_command_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file);

/**
 * Adds the binary directive line to the object file in octal format. This function converts the
 * binary representation of the directive line into an octal string and writes it to the object file
 * along with the instruction address.
 *
 * @param instructionLine A pointer to the `InstructionLine` structure that represents the directive
 *                        to be written to the object file.
 * @param object_file A pointer to the `FILE` object representing the object file where the directive
 *                    line will be written.
 */
void add_directive_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file);

/**
 * Opens a file with the specified name and mode. If the file cannot be opened, this function
 * prints an error message and exits the program.
 *
 * @param file_name A pointer to a string representing the name of the file to be opened.
 * @param mode A pointer to a string representing the mode in which to open the file (e.g., "r" for reading, "w" for writing).
 * @return A pointer to the `FILE` object if the file is successfully opened.
 * @throws The function will exit the program if the file cannot be opened.
 */
FILE* open_file(char *file_name, char *mode);

/**
 * Opens two files with the specified names, compares their contents, and prints whether they
 * are identical or different. The files are closed after comparison.
 *
 * @param file1_name A pointer to a string representing the name of the first file to be opened.
 * @param file2_name A pointer to a string representing the name of the second file to be opened.
 * @return An integer representing the result of the comparison: 0 if the files are identical, non-zero if they are different.
 */
int open_two_files_and_compare(char *file1_name, char *file2_name);

/**
 * @brief Searches for a specified string within a file and reports its presence.
 *
 * This function opens a file, reads its content line by line, and searches for the
 * occurrence of a given string within each line. If the string is found, it prints
 * the line number and the content of the line where the string occurs. If the string
 * is not found in the file, it reports that as well.
 *
 * @param filename The name of the file to be searched. It should be a valid, readable file.
 * @param search_str The string to search for within the file. It can be any valid C string.
 *
 * @return int
 * - Returns 1 if the string is found in the file.
 * - Returns 0 if the string is not found.
 * - Returns -1 if the file could not be opened or another error occurred.
 * */
int search_in_file(char *filename, const char *search_str);

#endif //ASSEMBLER_FILES_HANDLER_H


