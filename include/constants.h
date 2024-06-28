#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define MAX_MACRO_NAMES 150
#define MAX_LABEL_LENGTH 32
#define BINARY_WORD_LENGTH 15

#define IS_COMMAND 0
#define IS_DIRECTIVE 1

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
enum operand_classification_type {METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_STRUCT, METHOD_REGISTER, METHOD_UNKNOWN};
/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, NOT_DIRECTIVE};
/* Enum of commands ordered by their opcode */
enum opcode_command {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, NOT_OPCODE};



#endif //ASSEMBLER_CONSTANTS_H
