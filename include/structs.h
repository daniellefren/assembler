#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H
#include "constants.h"
#include "assembler.h"


#define INITIAL_IC 100
#define MAX_MACRO_LENGTH 100


typedef struct {
    char *line_content; // String containing the assembly instruction (content of the line)
    size_t length;  // Length of the line (excluding null terminator)
    int instruction_type; //is it directive or command
    unsigned int operand_number; //int containing the operand number
    char *first_operand; // string containing the first operand r0-r7(can be null)
    enum operand_classification_type first_operand_classification_type; // int containing enum values for first operand classification type
    char *second_operand; // string containing the second operand r0-r7(can be null)
    enum operand_classification_type second_operand_classification_type; // int containing enum values for second_operand classification type
    enum opcode_command opcode_command_type; // enum containing enum values for opcode command types if it's an opcode
    enum directives directive_type; // enum containing enum values for directive type if it's a directive
    int *data_values; // array of data values from the directive
    size_t data_values_count; // count of data values
    char *binary_instruction; //the line in binary instruction
} InstructionLine;


typedef struct {
    //struct of an array of line after the first run
    InstructionLine *lines;  // Pointer to an array of Line structs
    unsigned int number_of_line;  // Number of lines currently stored
    size_t capacity;  // Maximum capacity of the lines array
} LinesArray;

typedef struct {
    char label[MAX_LABEL_LENGTH];
    int type; //.data or .string
    char **value;
    size_t data_values_count; // count of data values
} Symbol;

typedef struct {
    Symbol *symbols;
    size_t size;
    size_t capacity;
} SymbolTable;

typedef struct {
    char* label;
    int type;
    char *value;
    size_t data_values_count; // count of data values
} Data;

typedef struct {
    Data *directive;
    size_t size;
} DataTable;


typedef struct {
    char name[MAX_LABEL_LENGTH];
    char body[MAX_MACRO_LENGTH][MAX_LINE_LENGTH];
    int lineCount;
} Macro;

typedef struct {
    Macro *macros;
    int count;
    int capacity;
} MacroTable;

typedef struct {
    char *command_name;
    int opcode;
    int num_of_operands;

} Command;


void free_lines_array(LinesArray *lines_array_pointer);

InstructionLine *init_instruction_line(InstructionLine *instruction_line_pointer);
void free_instruction_line(InstructionLine *instruction_line_pointer);
LinesArray *generate_instruction_line_array(int max_lines, LinesArray *my_lines_array);
char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line);
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line);
void addNewSymbol(SymbolTable *symbol_table, Symbol *symbol);
void init_lines_array(LinesArray *lines_array, int initial_capacity);
void init_symbol_table(SymbolTable *symbol_table, int initial_capacity);
#endif
