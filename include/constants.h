#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define MAX_MACRO_NAMES 150
#define MAX_LABEL_LENGTH 32

static const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

static const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};


#endif //ASSEMBLER_CONSTANTS_H
