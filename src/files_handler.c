#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../include/files_handler.h"
#include "../include/structs.h"
#include "../include/constants.h"

void create_output_files(InstructionLine instructionLine){
    
    return;
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