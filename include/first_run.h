#ifndef ASSEMBLER_FIRST_RUN_H
#define ASSEMBLER_FIRST_RUN_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "structs.h"
#include "files_handler.h"



/**
 * Performs the first pass over the assembly source file.
 * The function processes the assembly source file to handle macro expansion, symbol handling, Commands and directive processing in the provided assembly file
 * This function also updates the instruction counter (IC) and data counter (DC) based on the processed lines.
 * @param file The input assembly source file to be processed.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param lines_array A pointer to the LinesArray structure that stores all lines of the source file after processing.
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 * @param file_name The filename of the src assembly file
 */
void first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array, SymbolTable *symbol_table, int file_number, char *file_name);

/**
 * Pre-processes the assembly source file to expand macros.
 * performs the first pass over the assembly source file. It identifies macro definitions, stores them in a `MacroTable`,
 * and expands any macro invocations found in the source code. The expanded code is written to a new output file, which will be used in subsequent
 * assembly processing stages.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param file The input assembly source file to be processed.
 * @param new_file_name The name of the output file where the expanded assembly code (with macros expanded) will be written.
 * @return 1 if succeeded, else 0
 */
int pre_run(MacroTable *macro_table, FILE *file, char* new_file_name);

/**
 * Return 1 if macro definition start, 0 if not
 * @param line The line of assembly code to be processed.
 * @return 1 if True, else 0
 */
int is_macro_definition_start(char *line);

/**
 * @param line The line of assembly code to be processed.
 * @return 1 if True, else 0
 */
int is_macro_definition_end(char *line);

/**
 * Processes a single line of assembly code, handling symbols, commands, and directives.
 * @param line The line of assembly code to be processed.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param lines_array A pointer to the LinesArray structure that stores all lines of the source file after processing.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 * @param file_name The filename of the src assembly file
 * @return 1 if succeeded, else 1
 */
int read_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, LinesArray *lines_array, MacroTable *macro_table, int file_number, char *file_name);

/**
 * @brief Allocates memory and initializes a new symbol for the current instruction line.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the symbol will be associated with.
 * @param symbol_name The name of the symbol to be assigned to the new `symbol` structure.
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 * @return A pointer to the newly created `symbol` structure.
 */
Symbol* handle_symbol(InstructionLine *instruction_line, char *symbol_name, SymbolTable *symbol_table, int file_number);

/**
 * Processes a command line from the assembly source, extracting and classifying its operands.
 * The `handle_command` function analyzes a command line from the assembly source code, identifies the command,
 * extracts its operands, and classifies them according to their types.
 * The function updates the `InstructionLine` structure with the command information and calculates the number
 * of binary lines required for the instruction.
 * @param line The line of assembly code containing the command to be processed.
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the symbol will be associated with.
 */
int handle_command(char *line, SymbolTable *symbol_table, MacroTable *macro_table, InstructionLine *new_instruction_line);

/**
 * Get all operands from the current line and insert the data to operands in new_command
 * @param new_command A pointer to the `Command` structure New given Command
 * @param line The line of assembly code containing the directive to be processed.
 */
void define_operands_from_line(Command *new_command, char* line);

/**
 * Get second operand from line (if exists)
 * @param line The line of assembly code containing the directive to be processed.
 * @param new_command A pointer to the `Command` structure New given Command
 */
void extract_second_operand_from_line(char* line, Command *new_command);

/**
 * Defines the type of an operand based on its value.
 * @param operand A pointer to the `Operand` structure that contains the operand's value and type.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 */
int define_operand_types(Operand *operand, MacroTable *macro_table, SymbolTable *symbol_table);
/**
 * Function to check if a string is a valid symbol name
 * @param symbol Given symbol to check
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @return 1 if valid, else 0
 */
int is_valid_symbol(char *symbol, MacroTable *macro_table);

/**
 * Get number of binary lines for command based on the classification types of the operands
 * @param new_command A pointer to the `Command` structure New given Command
 * @return Integer that represents the number of lines
 */
int find_number_of_lines_in_binary(Command *new_command);

/**
 * Define the operand classification mode, and remove un needed characters ('*' and 'r' from resisters and # from integers)
 * @param new_operand New given operand
 * @return 1 if there were no errors, else 0
 */
int classify_operand(Operand *new_operand);

/**
 * Define the number of operands and opcode for the given command
 * @param command_name Given command name
 * @param new_command A pointer to the `Command` structure New given Command
 */
void get_operands_data_for_command(char* command_name, Command *new_command);

/**
 * Function to check if a line is a symbol
 * @param line The line of assembly code containing the directive to be processed.
 * @param symbol Given symbol to find
 * @return 1 if found, else 0
 */
int find_symbol(char *line, char *symbol);


/**
 * Find is key is a known assembly keyword
 * @param key Given key to check
 * @return 1 if symbol is an assembly keyword, else 0
 */
int is_known_assembly_keyword(char *key);

/**
 * Function to check if a line is a command
 * @param line The line of assembly code containing the `.string` directive.
 * @return 1 is line is a command, else 0
 */
int is_command(char *line);

/**
 * Function to check if the line contains a data directive
 * @param line The line of assembly code containing the directive to be processed.
 * @return 1 if directive, else 0
 */
int is_directive(char *line);

/**
 * Processes an assembly directive and updates the instruction line accordingly.
 * @param line The line of assembly code containing the directive to be processed.
 * @param dc Pointer to the data counter, which tracks the memory address of the data items.
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param ic Pointer to the instruction counter, which tracks the memory address of the commands.
 * @param file_number An integer representing the number of the current file being processed, used for generating unique output file names.
 * @param new_instruction_line A pointer to the `InstructionLine` structure that the symbol will be associated with.
 */
void handle_directives(char *line, int *dc, SymbolTable *symbol_table, int* ic, int file_number, InstructionLine *new_instruction_line, int has_symbol, Symbol *new_symbol);

/**
 * handle entry directive line - Mark symbol as entry in symbol table and add to entries file
 * @param new_directive A pointer to the `Directive` structure
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param line The line of assembly code containing the directive to be processed.
 */
void handle_entry_directive(Directive *new_directive, SymbolTable *symbol_table, char* line);

/**
 * handle extern directive line - Add extern symbol to symbol table and add to externals file
 * @param line The line of assembly code containing the directive to be processed.
 * @param new_directive A pointer to the `Directive` structure
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 */
void handle_extern_directive(char *line, Directive *new_directive, SymbolTable *symbol_table);

/**
 * Processes a `.string` directive and extracts the string value into the directive structure.
 * @param line The line of assembly code containing the `.string` directive.
 * @param new_directive A pointer to the `Directive` structure
 * @param instruction_line A pointer to the `InstructionLine` structure that will be updated with the binary line count for the string.
 */
void handle_string_directive(char *line, Directive *new_directive, InstructionLine *instruction_line);

/**
 * Processes a `.data` directive and extracts the integer values into the directive structure.
 * @param line The line of assembly code containing the `.data` directive.
 * @param new_directive A pointer to the `Directive` structure
 * @param instruction_line A pointer to the `InstructionLine` structure that will be updated with the binary line count for the data.
 */
void handle_data_directive(char *line, Directive *new_directive, InstructionLine *instruction_line);


/**
 * Add a new symbol to the SymbolTable.
 * The array is resized if needed.
 * @param symbol_table - Pointer to the SymbolTable structure.
 * @param symbol_name - Symbol name for given new symbol
 */
Symbol *add_new_symbol(SymbolTable *symbol_table, char* symbol_name);

/**
 * Make final actions in run:
 * 1. add to ic to all directory addresses because directories should be after command
 * 2. Define final ic and dc in lines array
 * 3. add entries to entries file
 * @param lines_array
 * @param ic
 * @param dc
 */
void final_actions(LinesArray *lines_array, int *ic, int *dc);

/**
 * Check if the operands of the command line are valid by specific rules
 * @param new_command A pointer to the `Command` structure New given Command
 * @param line The assembly src line that contins the given command
 * @return 1 if the command is valid, else 0
 */
int is_valid_operands_in_command_line(Command *new_command, char* line);

/**
 * Check if valid operands  by classification type for command with one operand
 * @param new_command A pointer to the `Command` structure New given Command
 * @param line The line of assembly code to be processed.
 * @return 1 if valid, else 0
 */
int check_one_operand_command(Command* new_command, char* line);

/**
 * Check if valid operands  by classification type for command with two operands
 * @param new_command A pointer to the `Command` structure New given Command
 * @param line The line of assembly code to be processed.
 * @return 1 if valid, else 0
 */
int check_two_operand_command(Command* new_command, char* line);

#endif
