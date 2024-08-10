#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../include/files_handler.h"


void create_ob_file(LinesArray *linesArray, int file_number){
    // TODO - add if condition when lines_array is empty
    char ob_file_name[100];
    int number_of_command;
    int number_of_directive;
    InstructionLine *p_line;
    add_number_to_string(ob_file_name, OBJECTS_FILE_NAME, sizeof(ob_file_name), file_number);
    FILE *object_file = open_ob_file(ob_file_name);

    number_of_command = linesArray->ic - STARTING_IC; //TODO problem with the ic and dc
    number_of_directive = linesArray->dc;

    add_first_line_to_ob_file(number_of_command, number_of_directive,object_file);

    for (int i = 0; i < linesArray->number_of_line-1; ++i) {
         p_line = &linesArray->lines[i];
        if (p_line == NULL) {
            exit(EXIT_FAILURE);
        }
        if (p_line->instruction_type == COMMAND){
            printf("Starting to insert command to object file address=%d, binary=%d, content=%s \n", p_line->starting_address, p_line->binary_line_count, p_line->line_content);
            add_command_lines_to_ob_file(p_line, object_file);
        }

    }
    for (int j = 0; j < linesArray->number_of_line-1; ++j) {
        p_line = &linesArray->lines[j];
        if (linesArray->lines[j].instruction_type == DATA_DIRECTIVE){
            printf("Starting to insert directive to object file address=%d, binary=%d, content=%s \n", p_line->starting_address, p_line->binary_line_count, p_line->line_content);
            add_directive_lines_to_ob_file(&linesArray->lines[j], object_file);
        }
    }
}

void add_entry_to_entries_file(char *label_name, int file_number, int label_address){
    char entries_file_name[100];
    add_number_to_string(entries_file_name, ENTRIES_FILE_NAME, sizeof(entries_file_name), file_number);
    FILE *file = fopen(entries_file_name, "w");
    if (file == NULL) {
        perror("Unable to open entries file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d\n", label_name, label_address);

    fclose(file);
}

void add_extern_to_externals_file(Label *label, int file_number, int *ic){
    char externals_file_name[100];
    add_number_to_string(externals_file_name, EXTERNALS_FILE_NAME, sizeof(externals_file_name), file_number);
    FILE *file = fopen(externals_file_name, "w");
    if (file == NULL) {
        perror("Unable to open externals file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d\n", label->name, *ic);

    fclose(file);
}

FILE* open_ob_file(char *ob_file_name){
    FILE *file = fopen(ob_file_name, "w");
    if (file == NULL) {
        perror("Unable to open entries file");
        exit(EXIT_FAILURE);
    }
    return file;
}
void add_first_line_to_ob_file(int number_of_command, int number_of_directive, FILE *object_file){
    if (object_file == NULL) {
        perror("Error: got a null file pointer");
        exit(EXIT_FAILURE);
    }
    fprintf(object_file, "%d %d\n", number_of_command, number_of_directive);
}

void add_command_lines_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    char *octal_number;
    int instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != COMMAND){
        perror("Error: this instruction line is not a command");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < instructionLine->binary_line_count; ++i) {
        octal_number = get_octal_base_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH);
        printf("The address is %d and the ocatal number is %s\n", instruction_address, octal_number);
        fprintf(object_file, "%d %s\n", instruction_address, octal_number);
        instruction_address += 1;
    }
}

void add_directive_lines_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    char *octal_number;
    int instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != DATA_DIRECTIVE){
        perror("Error: this instruction line is not a data directive");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < instructionLine->binary_line_count; ++i) {
        octal_number = get_octal_base_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH);
        printf("The address is %d and the ocatal number is %s\n", instruction_address, octal_number); //TODO Problem with convertion to octal number should add to the function the *ocatl_numer so it will change his value
        fprintf(object_file, "%d %s\n", instruction_address, octal_number);
        instruction_address += 1;
    }
}

int write_line_to_file(char *line, char* new_file_name) {
    FILE *outputFile = fopen(new_file_name, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n", new_file_name);
        exit(EXIT_FAILURE);
    }
//    lower_string(line);
    // Write the string to the file
    fprintf(outputFile, "%s", line);

    fclose(outputFile);
    return 0;  // Indicate success
}

void write_expanded_macros_to_file(MacroTable *macro_table, char* new_file_name) {
    int i;
    int j;
    FILE *outputFile = fopen(new_file_name, "a"); // Open in append mode
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open file '%s' for writing macros\n", new_file_name);
        exit(EXIT_FAILURE);
    }

    // Write each macro's body to the file
    for (i = 0; i < macro_table->count; ++i) {
        for (j = 0; j < macro_table->macros[i].lineCount; ++j) {
            fprintf(outputFile, "%s", macro_table->macros[i].body[j]);
        }
    }

    fclose(outputFile);
}