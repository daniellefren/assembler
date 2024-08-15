#include <stdlib.h>
#include <stdio.h>
#include "../include/assembler.h"

/**
 * Run the assembler on a given assembly source file.
 *
 * This function performs the following steps:
 * 1. Initializes the LinesArray and SymbolTable structures.
 * 2. Opens the specified assembly source file for reading.
 * 3. Executes the first run of the assembler to parse the file and populate the LinesArray and SymbolTable.
 * 6. Executes the second run of the assembler to generate the final output files (e.g., object file, externals, entries).
 * 7. Frees the memory allocated for the LinesArray and SymbolTable.
 *
 * @param assembly_lines_array - Pointer to the LinesArray structure that will hold the parsed assembly lines.
 * @param symbol_table - Pointer to the SymbolTable structure that will hold the symbols found during parsing.
 * @param filename - The name of the assembly source file to be processed.
 * @param file_number - An identifier number used to generate output file names.
 */

int main(int argc, char *argv[]) {
    LinesArray *assembly_lines_array = NULL;
    SymbolTable *symbol_table = NULL;
    char file_name[MAX_FILE_NAME_LEN];
    char final_file_name[MAX_FILE_NAME_LEN];
    int file_number;
    int number_of_files;
    number_of_files = argc - 1;
  
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    add_output_directory();
    delete_files_in_directory(OUTPUT_DIRECTORY_NAME);

    if(strcmp(argv[1], "test") == 0){
        output_test();
    }
    else{
        //Run assembler on all files
        for(file_number=1;file_number<=number_of_files;file_number++){
            strcpy(file_name, INPUT_DIRECTORY_NAME);
            strcat(file_name, "/");
            strcat(file_name, argv[file_number]);
            strcat(file_name, ".");
            strcat(file_name, SRC_FILE_NAME_EXTENSION);
            printf("filenamee %s\n", file_name);

            run_assembler_on_file(assembly_lines_array, symbol_table, file_name, file_number);
        }
    }

    return EXIT_SUCCESS;

}

void run_assembler_on_file(LinesArray *assembly_lines_array, SymbolTable *symbol_table, char* filename, int file_number){
    FILE *file;
    int ic, dc;

    ic = 0;
    dc = 0;

    assembly_lines_array = init_lines_array(10);
    symbol_table = init_symbol_table(10);

    // Open the file in read mode
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("error in run_assembler_on_file");
        print_internal_error(ERROR_CODE_48, filename);
        exit(EXIT_FAILURE);
    }

    // Call the first_run function with the file pointer
    first_run(file, &ic, &dc, assembly_lines_array, symbol_table, file_number, filename);

    // Close the file
    fclose(file);

    //call the second_run function with the LinesArray table
    start_second_run(assembly_lines_array, file_number, symbol_table, filename);

    free_lines_array(assembly_lines_array);
    free_symbol_table(symbol_table);
}

