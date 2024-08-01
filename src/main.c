#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"

//TODO - now the labels are not case sensitive
//TODO - handle ic and dc
// TODO - Add function that defines how many lines there is in an assembly command
int main(int argc, char *argv[]) {
    //TODO - add operand type
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

    printf("get all labels:\n");
    for(int i=0; i<symbol_table.size;i++){
        printf("label - %s, data :\n", symbol_table.symbols[i].label);
        for (size_t j = 0; j < symbol_table.symbols[i].data_values_count; j++) {
            printf("%s ", ((char **)symbol_table.symbols[i].value)[j]);
        }
    }
    printf("Get all instructions:\n");
    for(int k=0; k<2;k++){
        printf("line - \n");

        printf("%s \n", assembly_lines_array.lines[k].line_content);
        printf("first_operand - \n");
        printf("%s \n", assembly_lines_array.lines[k].first_operand);
        printf("second_operand - \n");
        printf("%s \n", assembly_lines_array.lines[k].second_operand);

    }


//    return_value = second_run();
//    printf("the value is %d", return_value);
    // Close the file
    fclose(file);



    return EXIT_SUCCESS;
}
