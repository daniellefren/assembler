#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H
#include "constants.h"


#define INITIAL_IC 100
#define MAX_MACRO_LENGTH 100

typedef struct {
    char symbol[MAX_SYMBOL_LENGTH];
    enum directives type; //.data or .string
    char **value;
    size_t data_values_count; // count of data values
} Directive;
//} Symbol;

typedef struct {
    char name[MAX_SYMBOL_LENGTH];
    enum instruction_types type; //command or data directive
    int address;
    int is_extern;
    int is_entry;
} Symbol;

typedef struct {
    Symbol *symbols;
    size_t size;
    size_t capacity;
} SymbolTable;

typedef struct {
    char *value;
    enum operand_types type; // symbol or register or number
    enum operand_classification_type classification_type; // int containing enum values for first operand classification type
    Symbol *symbol;
} Operand;

typedef struct {
    char *command_name;
    enum opcode_command opcode_command_type; // enum containing enum values for opcode command types if it's an opcode
    unsigned int operand_number; //int containing the operand number
    Operand *src_operand;
    Operand *dst_operand;
} Command;

typedef struct {
    char *line_content; // String containing the assembly instruction (content of the line)
    size_t length;  // Length of the line (excluding null terminator)
    enum instruction_types instruction_type; //is it data directive or command
    int is_symbol; // is there a symbol with the command
    Command *command;
    Directive *directive;
    Symbol *symbol;
    int starting_address;
    int binary_line_count; // the number of binary lines
    char *binary_instruction; //the line in binary instruction
} InstructionLine;


typedef struct {
    //struct of an array of line after the first run
    InstructionLine *lines;  // Pointer to an array of Line structs
    unsigned int number_of_line;  // Number of lines currently stored
    size_t capacity;  // Maximum capacity of the lines array
    int ic;
    int dc;
} LinesArray;


typedef struct {
    char name[MAX_SYMBOL_LENGTH];
    char body[MAX_MACRO_LENGTH][MAX_LINE_LENGTH];
    int lineCount;
} Macro;

typedef struct {
    Macro *macros;
    int count;
    int capacity;
} MacroTable;


void free_lines_array(LinesArray *lines_array_pointer);
void free_instruction_line(InstructionLine *instruction_line_pointer);
void free_macro_table(MacroTable *macro_table);
void free_symbol_table(SymbolTable *symbol_table);

/**
 * Frees the memory allocated for the binary instruction in an InstructionLine.
 *
 * @param p_line Pointer to the InstructionLine structure.
 */
void free_binary_instruction(InstructionLine *p_line);


void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line);
void add_new_symbol(SymbolTable *symbol_table, Symbol *new_symbol);
LinesArray *init_lines_array(int initial_capacity);
SymbolTable * init_symbol_table(int initial_capacity);
void init_macro_table(MacroTable *table);
InstructionLine *init_instruction_line(char* line);
Command *init_command();
Operand *init_operand();
Directive *init_directive();
void init_macro_name_array(char **macroNames);

/**
 * Allocates memory for storing a binary instruction in the provided InstructionLine.
 *
 * This function calculates the required memory size based on the number of binary lines
 * and the length of each binary line. It then attempts to allocate that memory. If the
 * allocation fails, the program prints an error message and exits. If `binary_line_count`
 * is zero, the function prints an error and returns without allocating memory.
 *
 * @param p_line A pointer to an InstructionLine structure where the binary instruction
 *               memory will be allocated.
 * @param binary_line_count The number of binary lines to be allocated.
 * @param binary_line_length The length of each binary line.
 */
void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length);

/**
 * Retrieves the binary instruction from a specific line in the LinesArray.
 *
 * This function returns a pointer to the binary instruction stored in the
 * specified line of the LinesArray. It's assumed that the line index provided
 * is within the valid range of the LinesArray.
 *
 * @param linesArray A pointer to the LinesArray structure that contains the lines.
 * @param number_of_line The index of the line from which to retrieve the binary instruction.
 * @return A pointer to the binary instruction stored in the specified line.
 */
char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line);


#endif
