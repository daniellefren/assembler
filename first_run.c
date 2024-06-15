#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include "first_run.h"
#include "assembler.h"

const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "stop"
};

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};


bool first_run(FILE *file, int *ic, int *dc, LinesArray *lines_array){
    char line[MAX_LINE_LENGTH];
    int line_num = 1;


    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if(!ignore_line(line)){
            read_line(line);
        }

        line_num++;
    }
}

void read_line(const char *line){
    const char *directive;
    char *label = NULL;
    bool hasLabel;

    printf("%s\n", line);

    line = skip_spaces(line);

    hasLabel = find_label(line, label);
    printf("label %s\n", label);

    if(hasLabel){
        line = strchr(line, ':') + 1;
    }


    if((directive = find_directive(line)) != NULL){
        printf("directive %s\n", directive);
    }

    if(is_macro(line)){
        printf("macro\n");
    }
    if(is_operand(line)){
        printf("operand\n");
    }

}

int ignore_line(char *line){
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

const char* skip_spaces(const char *line){
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

// Function to check if a line is a label
bool find_label(const char *line, char *label) {
    // Function to check if a line is a label and return the label
    // Skip leading whitespace
    while (*line && isspace((unsigned char) *line)) {
        line++;
    }

    // Start of the potential label
    const char *start = line;

    // Find the end of the label (colon or whitespace)
    while (*line && !isspace((unsigned char) *line) && *line != ':') {
        line++;
    }

    // Check if the character after the label is a colon
    if (*line == ':') {
        size_t len = line - start;
        strncpy(label, start, len);
        label[len] = '\0'; // Null-terminate the label
        return true;
    }

    return false;
}



// Function to check if a line is a macro
bool is_macro(const char *line) {
    // A macro in NASM typically starts with "%macro"
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return strncmp(line, "%macro", 6) == 0;
}

// Function to check if a line is an operand (simple check)
bool is_operand(const char *line) {
    // An operand line typically starts with an instruction or mnemonic
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (strncmp(line, commands[i], strlen(commands[i])) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a line is a directive
const char *find_directive(const char *line) {
    // Valid directives
    const char* directive = NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    for (int i = 0; i < sizeof(directives)/sizeof(directives[0]); i++) {
        if (strncmp(line, directives[i], strlen(directives[i])) == 0) {
            directive = directives[i];
        }
    }

    return directive;
}