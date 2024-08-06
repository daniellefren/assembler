#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define MAX_MACRO_NAMES 150
#define MAX_LABEL_LENGTH 31
#define BINARY_WORD_LENGTH 15
#define MAX_COMMAND_LEN 10
#define MAX_OPERAND_LEN 20

#define IS_COMMAND 0
#define IS_DIRECTIVE 1
#define OPCODE_SIZE 4
#define MAX_OPERAND_SIZE 10


static const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

static const char *directives[] = {
        "data", "string", "struct", "entry", "extern"
};

#define COMMANDS_COUNT (sizeof(commands) / sizeof(commands[0]))
#define DIRECTIVES_COUNT (sizeof(directives) / sizeof(directives[0]))
#define TOTAL_ASSEMBLY_KEYWORDS_COUNT (COMMANDS_COUNT + DIRECTIVES_COUNT)

/* A/R/E modes ordered by their numerical value */
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};
/* Addressing methods ordered by their code */
enum operand_classification_type {IMMEDIATE = 0, DIRECT = 1, INDIRECT_REGISTER = 2, DIRECT_REGISTER = 3, METHOD_UNKNOWN = -1};
/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, NOT_DIRECTIVE};
/* Enum of commands ordered by their opcode */
enum opcode_command {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, NOT_OPCODE};

enum instruction_types {
    COMMAND = 1,
    DATA_DIRECTIVE = 2,
    EXTERN_DIRECTIVE = 3
};

enum operand_types {
    REGISTER = 0,
    INTEGER = 1,
    LABEL = 2,
    INVALID = 3
};




#endif //ASSEMBLER_CONSTANTS_H
