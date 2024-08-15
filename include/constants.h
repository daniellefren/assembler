#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define STARTING_IC 100
#define MAX_MACRO_NAMES 150
#define MAX_SYMBOL_LENGTH 31
#define MAX_FILE_NAME_LEN 100
#define BINARY_WORD_LENGTH 15
#define MAX_COMMAND_LEN 10
#define MAX_OPERAND_LEN 20
#define OCTAL_LENGTH 5
#define IS_COMMAND 0
#define IS_DIRECTIVE 1
#define OPCODE_SIZE 4
#define MAX_OPERAND_SIZE 10
#define MAX_LINE_LENGTH 80
#define SRC_OPERAND_NUMBER 1
#define DST_OPERAND_NUMBER 2
#define ARE_STARTING_OFFSET 11
#define SRC_REGISTER_OFFSET 6
#define DST_REGISTER_OFFSET 9
#define BINARY_INSTRUCTION_MAX_LENGTH 46
#define BINARY_LINE_LENGTH 15
#define SYMBOL_NAME_LEN 15

static const char *COMMANDS[] = {
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

static const char *DIRECTIVES[] = {
        "data", "string", "struct", "entry", "extern"
};

static const char *OUTPUT_DIRECTORY_NAME = "output_files";
static const char *ERROR_FILE_NAME = "output_files/error_logs%d";


static const char *EXTERNALS_FILE_NAME = "output_files/ps%d.ext";
static const char *ENTRIES_FILE_NAME = "output_files/ps%d.ent";
static const char *EXPANDED_MACRO_FILE_NAME = "output_files/expanded_macros%d.am";
static const char *OBJECTS_FILE_NAME = "output_files/ps%d.ob";
static const char *EXTERNALS_FILE_EXTENSION = "ext";
static const char *ENTRIES_FILE_EXTENSION = "ent";
static const char *OBJECT_FILE_EXTENSION = "ob";
static const char *EXPENDED_MACROS_EXTENSION = "am";


#define COMMANDS_COUNT (sizeof(COMMANDS) / sizeof(COMMANDS[0]))
#define DIRECTIVES_COUNT (sizeof(DIRECTIVES) / sizeof(DIRECTIVES[0]))
#define TOTAL_ASSEMBLY_KEYWORDS_COUNT (COMMANDS_COUNT + DIRECTIVES_COUNT)


/* A/R/E modes ordered by their numerical value */
enum ARE {ABSOLUTE, EXTERNAL, RELOCATABLE};
/* Addressing methods ordered by their code */
enum operand_classification_type {IMMEDIATE = 0, DIRECT = 1, INDIRECT_REGISTER = 2, DIRECT_REGISTER = 3, METHOD_UNKNOWN = -1};
/* Directives types */
enum directives {DATA, STRING, STRUCT, ENTRY, EXTERN, NOT_DIRECTIVE};
/* Enum of commands ordered by their opcode */
enum opcode_command {MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, NOT_OPCODE};

enum instruction_types {
    COMMAND = 1,
    DATA_DIRECTIVE = 2,
    EXTERN_DIRECTIVE = 3,
    ENTRY_DIRECTIVE = 4
};

enum operand_types {
    REGISTER = 0,
    INTEGER = 1,
    SYMBOL = 2,
    INVALID = 3
};





#endif //ASSEMBLER_CONSTANTS_H
