
#include "../include/constants.h"

const char *INPUT_DIRECTORY_NAME = "input_files";
const char *OUTPUT_DIRECTORY_NAME = "output_files";
const char *EXTERNALS_FILE_EXTENSION = "ext";
const char *ENTRIES_FILE_EXTENSION = "ent";
const char *OBJECT_FILE_EXTENSION = "ob";
const char *EXPENDED_MACROS_EXTENSION = "am";
const char *SRC_FILE_NAME_EXTENSION = "as";
const char *COMMANDS[] = {
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

const char *DIRECTIVES[] = {
        "data", "string", "struct", "entry", "extern"
};
