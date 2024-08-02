#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define MAX_MACRO_NAMES 150
#define MAX_LABEL_LENGTH 32
#define BINARY_WORD_LENGTH 15
#define MAX_COMMAND_LEN 10

#define IS_COMMAND 0
#define IS_DIRECTIVE 1
#define OPCODE_SIZE 4
#define MAX_OPERAND_SIZE 10


static const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

static const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

/* A/R/E modes ordered by their numerical value */
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};
/* Addressing methods ordered by their code */
enum operand_classification_type {IMMEDIATE = 0, DIRECT = 1, INDIRECT_REGISTER = 2, DIRECT_REGISTER = 3, METHOD_UNKNOWN = -1};
/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, NOT_DIRECTIVE};
/* Enum of commands ordered by their opcode */
enum opcode_command {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, NOT_OPCODE};

enum opcode_command_operands {
    MOV_OPERANDS = 2,    // MOV instruction uses 2 operands
    CMP_OPERANDS = 2,    // CMP instruction uses 2 operands
    ADD_OPERANDS = 2,    // ADD instruction uses 2 operands
    SUB_OPERANDS = 2,    // SUB instruction uses 2 operands
    NOT_OPERANDS = 1,    // NOT instruction uses 1 operand
    CLR_OPERANDS = 1,    // CLR instruction uses 1 operand
    LEA_OPERANDS = 2,    // LEA instruction uses 2 operands
    INC_OPERANDS = 1,    // INC instruction uses 1 operand
    DEC_OPERANDS = 1,    // DEC instruction uses 1 operand
    JMP_OPERANDS = 1,    // JMP instruction uses 1 operand
    BNE_OPERANDS = 1,    // BNE instruction uses 1 operand
    RED_OPERANDS = 1,    // RED instruction uses 1 operand
    PRN_OPERANDS = 1,    // PRN instruction uses 1 operand
    JSR_OPERANDS = 1,    // JSR instruction uses 1 operand
    RTS_OPERANDS = 0,    // RTS instruction uses 0 operands
    STOP_OPERANDS = 0,   // STOP instruction uses 0 operands
};

enum instruction_types {
    COMMAND = 1,
    DATA_DIRECTIVE = 2
};

enum operand_types {
    REGISTER = 0,
    INTEGER = 1,
    LABEL = 2,
    INVALID = 3
};




#endif //ASSEMBLER_CONSTANTS_H
