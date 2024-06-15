




typedef struct {
    char *instruction; // String containing the assembly instruction
    unsigned int operand_number;
    char *first_operand;
    char *second_operand;
    int command_type;
    int classification_type;

    // Add more fields if needed, like operand types, etc.
} InstructionLine;