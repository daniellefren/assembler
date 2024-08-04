#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/utils.h"



void lower_string(char *string){
// Convert command_name to lowercase
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}

// Function to trim leading and trailing spaces
char *trim_spaces(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size) {
    /*
     In calling function
    char appended_string[100];  // Pre-allocate memory for combined string
    string_append(string1, string2, appended_string, sizeof(appended_string_size));
    */
    size_t first_string_size = strlen(first_string);
    size_t second_string_size = strlen(second_String);
    if (first_string_size + second_string_size + 1 > appended_string_size) {
        // Handle insufficient space in destination (e.g., return error code)
        return;
    }

    // Copy s1 and s2 using strcpy and strcat
    strcpy(appended_string, first_string);
    strcat(appended_string, second_String);
}

void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length) {
    size_t count_to_allocate = binary_line_count * binary_line_length + 1; // Calculate total size in bytes
    printf("binary_line_count - %lu\n", binary_line_count);
    if (binary_line_count == 0){//TODO - delete after danielle add binary_line_count logic
        count_to_allocate = 46;
    }
    p_line->binary_instruction = (char*) malloc(count_to_allocate);
    printf("The size wanted to allocate is %lu\n", count_to_allocate);
    if (p_line->binary_instruction == NULL) {
        fprintf(stderr, "Memory allocation failed for binary instruction\n");
        exit(1);
    }
}

void free_binary_instruction(InstructionLine *p_line) {
    free(p_line->binary_instruction);
}

void print_command(Command *command){
    printf("Command name is %s\n", command->command_name);
    if (command->operand_number > 0){
        printf("Src Operand value is %s and the classification type is %d\n", command->src_operand->value, command->src_operand->classification_type);
    }
    else{
        printf("No Operands :(\n");
    }
    if (command->operand_number > 1){
        printf("Dst Operand value is %s and the classification type is %d\n", command->dst_operand->value, command->dst_operand->classification_type);
    }

}
void print_directive(Directive *directive){
    printf("The Lenth of the directive is %zu and the value is %s\n", directive->data_values_count, *directive->value);
}

void print_instruction_line(InstructionLine *instructionLine){
    printf("The Content of line is %s\n", instructionLine->line_content);
    if (instructionLine->command != NULL){
        print_command(instructionLine->command);
    }
    else if (instructionLine->directive != NULL){
        print_directive(instructionLine->directive);
    }
}
