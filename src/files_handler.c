#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/files_handler.h"
#include "../include/errors.h"

void create_ob_file(LinesArray *linesArray, int file_number){
    char ob_file_name[100];
    int number_of_command;
    int number_of_directive;
    FILE *object_file;

    if (linesArray == NULL){
//        fprintf(stderr, "Error: Lines array is NULL\n");
        print_internal_error(ERROR_CODE_28, "");
        exit(EXIT_FAILURE);
    }

    // Generate the object file name with the file number appended
    add_number_to_string(ob_file_name, OBJECTS_FILE_NAME, sizeof(ob_file_name), file_number);
    object_file = open_ob_file(ob_file_name);
    number_of_command = linesArray->ic - STARTING_IC;
    number_of_directive = linesArray->dc;

    add_first_line_to_ob_file(number_of_command, number_of_directive, object_file);
    add_all_command_lines_to_ob_file(linesArray, object_file);
    add_all_directive_lines_to_ob_file(linesArray, object_file);
    printf("Finished creating object file\n");
}

FILE* open_ob_file(char *ob_file_name) {
    FILE *file;

    // Attempt to open the file in write mode ("w" will create a new file or overwrite an existing file)
    file = fopen(ob_file_name, "w");
    if (file == NULL) {
        perror("Unable to open entries file");
        exit(EXIT_FAILURE);
    }
    return file;
}


void add_first_line_to_ob_file(int number_of_command, int number_of_directive, FILE *object_file) {
    if (object_file == NULL) {
        perror("Error: got a null file pointer");
        exit(EXIT_FAILURE);
    }

    // Write the number of commands and directives to the first line of the file
    fprintf(object_file, "%d %d\n", number_of_command, number_of_directive);
}


void add_all_command_lines_to_ob_file(LinesArray *lines_array, FILE *object_file){
    InstructionLine *p_line;
    // Iterate over all lines in the lines array
    for (int i = 0; i < lines_array->number_of_line; ++i) {
        p_line = &lines_array->lines[i];
        if (p_line == NULL) {
//            fprintf(stderr, "Error: Line number %d in lines array is NULL\n", i);
            print_internal_error(ERROR_CODE_29, int_to_string(i));
            continue;
        }

        if (p_line->instruction_type == COMMAND) {
            printf("Starting to insert command to object file address=%d, binary=%d, content=%s\n",
                   p_line->starting_address, p_line->binary_line_count, p_line->line_content);
            add_command_line_to_ob_file(p_line, object_file);
        }
    }
}


void add_all_directive_lines_to_ob_file(LinesArray *lines_array, FILE *object_file){
    InstructionLine *p_line;
    // Iterate over all lines in the lines array
    for (int i = 0; i < lines_array->number_of_line; ++i) {
        p_line = &lines_array->lines[i];
        if (p_line == NULL) {
//            fprintf(stderr, "Error: Line number %d in lines array is NULL\n", i);
            print_internal_error(ERROR_CODE_30, int_to_string(i));
            continue;
        }

        if (lines_array->lines[i].instruction_type == DATA_DIRECTIVE) {
           printf("Starting to insert directive to object file address=%d, binary=%d, content=%s\n",
                   p_line->starting_address, p_line->binary_line_count, p_line->line_content);
           add_directive_line_to_ob_file(p_line, object_file);
        }
    }
}



void add_command_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    char *octal_number;            // Pointer to hold the octal string representation of the binary instruction
    int instruction_address;       // Variable to store the starting address of the instruction

    // Allocate memory for the octal number string (5 digits + null terminator)
    octal_number = (char *)malloc((OCTAL_LENGTH + 1) * sizeof(char));
    if (octal_number == NULL) {
        perror("Error: Unable to allocate memory for octal_number");
        exit(EXIT_FAILURE);
    }

    instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != COMMAND){
        perror("Error: this instruction line is not a command");
        exit(EXIT_FAILURE);
    }

    // Loop through each binary line in the instruction
    for (int i = 0; i < instructionLine->binary_line_count; ++i) {
        // Convert the binary instruction to an octal string
        fill_octal_string_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH, octal_number);
        printf("The address is %d and the octal number is %s\n", instruction_address, octal_number);
        fprintf(object_file, "%d %s\n", instruction_address, octal_number);
        instruction_address += 1;
    }
    free(octal_number);
}

void add_directive_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    char *octal_number;            // Pointer to hold the octal string representation of the binary instruction
    int instruction_address;       // Variable to store the starting address of the instruction

    // Allocate memory for the octal number string (5 digits + null terminator)
    octal_number = (char *)malloc((OCTAL_LENGTH + 1) * sizeof(char));
    if (octal_number == NULL) {
        perror("Error: Unable to allocate memory for octal_number");
        exit(EXIT_FAILURE);
    }

    instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != DATA_DIRECTIVE){
        perror("Error: this instruction line is not a data directive");
        exit(EXIT_FAILURE);
    }

    // Loop through each binary line in the instruction
    for (int i = 0; i < instructionLine->binary_line_count; ++i) {
        // Convert the binary instruction to an octal string
        fill_octal_string_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH, octal_number);
        printf("The address is %d and the octal number is %s\n", instruction_address, octal_number);
        fprintf(object_file, "%d %s\n", instruction_address, octal_number);
        instruction_address += 1;
    }
}

void add_entry_to_entries_file(char *symbol_name, int file_number, int symbol_address){
    char entries_file_name[100];
    FILE *file;

    add_number_to_string(entries_file_name, ENTRIES_FILE_NAME, sizeof(entries_file_name), file_number);
    file = fopen(entries_file_name, "w");
    if (file == NULL) {
        perror("Unable to open entries file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d\n", symbol_name, symbol_address);

    fclose(file);
}



void add_extern_to_externals_file(Symbol *symbol, int file_number, int *ic){
    char externals_file_name[100];
    add_number_to_string(externals_file_name, EXTERNALS_FILE_NAME, sizeof(externals_file_name), file_number);
    FILE *file = fopen(externals_file_name, "w");
    if (file == NULL) {
        perror("Unable to open externals file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d\n", symbol->name, *ic);

    fclose(file);
}




int write_line_to_file(char *line, char* new_file_name) {
    FILE *outputFile = fopen(new_file_name, "a"); // Open in append mode
    if (!outputFile) {
//        fprintf(stderr, "Error: Could not open file '%s' for writing\n", new_file_name);
        print_internal_error(ERROR_CODE_48, new_file_name);
        exit(EXIT_FAILURE);
    }
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
//        fprintf(stderr, "Error: Could not open file '%s' for writing macros\n", new_file_name);
        print_internal_error(ERROR_CODE_49, new_file_name);
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

void add_output_directory(){
    const char *dirName = OUTPUT_DIRECTORY_NAME;
    struct stat st = {0};

    // Check if the directory exists
    if (stat(dirName, &st) == -1) {
        // Directory does not exist, create it
        if (mkdir(dirName, 0755) == 0) {
            printf("Directory '%s' created successfully.\n", dirName);
        } else {
            perror("Error creating directory");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Directory '%s' already exists.\n", dirName);
    }
}