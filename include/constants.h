#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

enum Command_Types {
    OPCODE = 1,
    DATA = 2,
    EXTERN = 3,
    STRING = 4,
    ENTRY = 5
};

enum Classification_Types {
    Immediate_Addressing = 0,
    Direct_Addressing = 1,
    Indirect_Register_Addressing = 2,
    Direct_Register_Addressing = 3
};


#endif //ASSEMBLER_CONSTANTS_H
