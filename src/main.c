#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"


int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0;
    int return_value;
    LinesArray assembly_lines_array;
    SymbolTable symbol_table;


    assembly_lines_array.lines = NULL;
    assembly_lines_array.number_of_line = 0;
    assembly_lines_array.capacity = 0;


    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the file in read mode
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }


    // Call the first_run function with the file pointer
    first_run(file, &ic, &dc, &assembly_lines_array, &symbol_table);
    //call the second_run function with the LinesArray table
//    start_second_run(assembly_lines_array);


//    return_value = second_run();
//    printf("the value is %d", return_value);
    // Close the file
    fclose(file);



    return EXIT_SUCCESS;
}
