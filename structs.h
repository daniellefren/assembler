typedef struct {
    char *line_content; // String containing the assembly instruction (content of the line)
    size_t length;  // Length of the line (excluding null terminator)
    unsigned int operand_number; //int containing the operand number
    char *first_operand; // string containing the first operand r0-r7(can be null)
    char *second_operand; // string containing the second operand r0-r7(can be null)
    int command_types; // int containing enum values for command types
    int classification_type; // int containing enum values for classification type
} InstructionLine;

typedef struct {
    //struct of an array of line after the first run
    InstructionLine *lines;  // Pointer to an array of Line structs
    unsigned int number_of_line;  // Number of lines currently stored
    size_t capacity;  // Maximum capacity of the lines array
} LinesArray;
