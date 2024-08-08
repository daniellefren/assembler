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
} Directive;
//} Symbol;

typedef struct {
    char name[MAX_LABEL_LENGTH];
    enum instruction_types type; //command or data directive
    int address;
    int is_extern;
    int is_entry;
} Label;

typedef struct {
    Label *labels;
    size_t size;
    size_t capacity;
} LabelTable;

typedef struct {
    char *value;
    enum operand_types type; // Label or register or number TODO - change to enum
    enum operand_classification_type classification_type; // int containing enum values for first operand classification type
    Label *label;
} Operand;

typedef struct {
    char *command_name;
    enum opcode_command opcode_command_type; // enum containing enum values for opcode command types if it's an opcode
    unsigned int operand_number; //int containing the operand number
    Operand *src_operand; // TODO - change so it will work
    Operand *dst_operand;
} Command;

typedef struct {
    char *line_content; // String containing the assembly instruction (content of the line)
    size_t length;  // Length of the line (excluding null terminator)
    enum instruction_types instruction_type; //is it data directive or command
    int is_label; // is there a label with the command
    Command *command;
    Directive *directive;
    Label *label;
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

void free_instruction_line(InstructionLine *instruction_line_pointer);
char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line);
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line);
void addNewLabel(LabelTable *label_table, Label *new_label);
LinesArray *init_lines_array(int initial_capacity);
LabelTable * init_label_table(int initial_capacity);
void init_macro_table(MacroTable *table);
InstructionLine *init_instruction_line(char* line);
Command *init_command();
Operand *init_operand();
Directive *init_directive();
void init_macro_name_array(char **macroNames);


#endif
