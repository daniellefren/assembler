#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"
#include "../include/utils.h"

void run_assembler_on_file(LinesArray *assembly_lines_array, SymbolTable *symbol_table, char* filename, int file_number);

int main(int argc, char *argv[]) {
    LinesArray *assembly_lines_array;
    SymbolTable *symbol_table;
    int number_of_files = argc - 1;

    // Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    add_output_directory();

    //Run assembler on all files
    for(int file_number=1;file_number<=number_of_files;file_number++){
        run_assembler_on_file(assembly_lines_array, symbol_table, argv[file_number], file_number);
    }

    return EXIT_SUCCESS;
}

void run_assembler_on_file(LinesArray *assembly_lines_array, SymbolTable *symbol_table, char* filename, int file_number){
    FILE *file;
    int ic, dc = 0;
    assembly_lines_array = init_lines_array(10);
    symbol_table = init_symbol_table(10);

    // Open the file in read mode
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Call the first_run function with the file pointer
    first_run(file, &ic, &dc, assembly_lines_array, symbol_table, file_number);

    test_all_run(assembly_lines_array); //TODO - delete

    // Close the file
    fclose(file);

    //call the second_run function with the LinesArray table
    start_second_run(assembly_lines_array, file_number);

    free_lines_array(assembly_lines_array);
    free_symbol_table(symbol_table);
}

