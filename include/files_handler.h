
#ifndef ASSEMBLER_FILES_HANDLER_H
#define ASSEMBLER_FILES_HANDLER_H
#include "../include/utils.h"


/**
 * Create an output directory if it does not already exist.
 */
void add_output_directory();


/**
 * Add an external symbol to the externals file.
 *
 * @param symbol - Pointer to the Symbol struct representing the external symbol.
 * @param file_number - The file number used to generate the externals file name.
 * @param ic - Pointer to the instruction counter (IC) which indicates the address of the symbol.
 */
void add_extern_to_externals_file(Symbol *symbol, int file_number, int *ic);

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
#endif //ASSEMBLER_FILES_HANDLER_H


