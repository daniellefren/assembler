#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"


int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0;
    LinesArray lines_array;
    SymbolTable symbol_table;


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
    first_run(file, &ic, &dc, &lines_array, &symbol_table);

    int return_value = second_run();
    printf("the value is %d", return_value);
    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}
