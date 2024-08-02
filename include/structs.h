#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H
#include "constants.h"
#include "assembler.h"


#define INITIAL_IC 100
#define MAX_MACRO_LENGTH 100

typedef struct {
    char label[MAX_LABEL_LENGTH];
    enum directives type; //.data or .string
    char **value;
    size_t data_values_count; // count of data values
} Directive
//} Symbol;

typedef struct {
    char name[MAX_LABEL_LENGTH];
    enum instruction_types type; //command or data directive
    int address;
} Label

typedef struct {
    Symbol *labels;
    size_t size;
    size_t capacity;
} LabelTable

typedef struct {
    char *value;
    int type; // Label or register or number TODO - change to enum
    enum operand_classification_type classification_type; // int containing enum values for first operand classification type
    Symbol symbol;
} Operand;

typedef struct {
    char *command_name;
    enum opcode_command opcode_command_type; // enum containing enum values for opcode command types if it's an opcode
    unsigned int operand_number; //int containing the operand number
    Operand src_operand; // TODO - change so it will work
    Operand dst_operand;
} Command;

typedef struct {
    char *line_content; // String containing the assembly instruction (content of the line)
    size_t length;  // Length of the line (excluding null terminator)
    int instruction_type; //is it data directive or command
    int is_label; // is there a label with the command
    Command command;
    Directive directive;
    Label label;
    char *binary_instruction; //the line in binary instruction
} InstructionLine;


typedef struct {
    //struct of an array of line after the first run
    InstructionLine *lines;  // Pointer to an array of Line structs
    unsigned int number_of_line;  // Number of lines currently stored
    size_t capacity;  // Maximum capacity of the lines array
} LinesArray;





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


void free_lines_array(LinesArray *lines_array_pointer);

InstructionLine *init_instruction_line(InstructionLine *instruction_line_pointer);
void free_instruction_line(InstructionLine *instruction_line_pointer);
LinesArray *generate_instruction_line_array(int max_lines, LinesArray *my_lines_array);
char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line);
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line);
void addNewSymbol(SymbolTable *symbol_table, Symbol *symbol);
void init_lines_array(LinesArray *lines_array, int initial_capacity);
void init_label_table(SymbolTable *symbol_table, int initial_capacity);
#endif
