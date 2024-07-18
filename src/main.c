#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"

//TODO - now the labels are not case sensitive

int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0;
    int return_value;


    LinesArray assembly_lines_array; // Use struct, not a pointer
    SymbolTable symbol_table;


    init_symbol_table(&symbol_table, 10);
    init_lines_array(&assembly_lines_array, 10);


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

    for(int i=0; i<3;i++){
        printf("label - %s, data :\n", symbol_table.symbols[i].label);
        for (size_t j = 0; j < symbol_table.symbols[i].data_values_count; j++) {
            printf("%s ", ((char **)symbol_table.symbols[i].value)[j]);
        }
    }


//    return_value = second_run();
//    printf("the value is %d", return_value);
    // Close the file
    fclose(file);



    return EXIT_SUCCESS;
}
