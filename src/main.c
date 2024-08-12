#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"

void add_output_directory();

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

//TODO - get over al the todos
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
        //TODO - create directory for each run (?)
        FILE *file;
        int ic, dc = 0;
        assembly_lines_array = init_lines_array(10);
        symbol_table = init_symbol_table(10);

        // Open the file in read mode
        file = fopen(argv[file_number], "r");
        if (file == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }


        // Call the first_run function with the file pointer
        first_run(file, &ic, &dc, assembly_lines_array, symbol_table, file_number);

        test_all_run(assembly_lines_array); //TODO - delete

        // Close the file
        fclose(file);

        //call the second_run function with the LinesArray table
//        start_second_run(assembly_lines_array);

//        free_lines_array(assembly_lines_array);
//        free_symbol_table(symbol_table);
    }

    return EXIT_SUCCESS;
}
