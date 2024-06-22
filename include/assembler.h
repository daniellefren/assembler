#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#define MAX_LINE_LENGTH 80
#define NO_ERROR 0
#define ERROR -1



/* A/R/E modes ordered by their numerical value */
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};
/* Addressing methods ordered by their code */
enum methods {METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_STRUCT, METHOD_REGISTER, METHOD_UNKNOWN};
/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, UNKNOWN_TYPE};
/* Enum of commands ordered by their opcode */
enum commands {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, UNKNOWN_COMMAND};

#endif //ASSEMBLER_ASSEMBLER_H
