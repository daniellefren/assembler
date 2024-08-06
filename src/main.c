#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"

//TODO - handle ic and dc
// TODO - Add function that defines how many lines there is in an assembly command
int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0;


    LinesArray *assembly_lines_array = init_lines_array(10);
    LabelTable *label_table = init_label_table(10);


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
    first_run(file, &ic, &dc, assembly_lines_array, label_table);

    dc += ic;
    test_all_run(assembly_lines_array);
    //call the second_run function with the LinesArray table



    // Close the file
    fclose(file);

    for (int i = 0; i < assembly_lines_array->number_of_line; ++i) {
        printf("Line number %d: %s \n",i, assembly_lines_array->lines[i].line_content);
    }

    start_second_run(assembly_lines_array);

    //return EXIT_SUCCESS;
    return 1;
}
