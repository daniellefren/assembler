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
    //TODO - delete all output files from the previouse run
    LinesArray *assembly_lines_array = NULL;
    SymbolTable *symbol_table = NULL;
    int file_number;
    int number_of_files;
    number_of_files = argc - 1;
  
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    add_output_directory();
    output_test("","");
    /*
    //Run assembler on all files
    for(file_number=1;file_number<=number_of_files;file_number++){
        run_assembler_on_file(assembly_lines_array, symbol_table, argv[file_number], file_number);
    }
    */

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
    first_run(file, &ic, &dc, assembly_lines_array, symbol_table, file_number);

//    test_all_run(assembly_lines_array); //TODO - delete

    // Close the file
    fclose(file);

    //call the second_run function with the LinesArray table
    start_second_run(assembly_lines_array, file_number, symbol_table);

    free_lines_array(assembly_lines_array);
    free_symbol_table(symbol_table);
}

