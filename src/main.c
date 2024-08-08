#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"
#include "../include/files_handler.h"
//TODO - handle ic and dc
// TODO - Add function that defines how many lines there is in an assembly command
// TODO - Add function that read from directory and not files
int main(int argc, char *argv[]) {

    int number_of_files = argc - 1;

    // Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    printf("number_of_files %d\n", number_of_files);
    //Run assembler on all files
    for(int file_number=1;file_number<=number_of_files;file_number++){
        FILE *file;
        int ic, dc = 0;
        LinesArray *assembly_lines_array = init_lines_array(10);
        LabelTable *label_table = init_label_table(10);

        // Open the file in read mode
        file = fopen(argv[file_number], "r");
        if (file == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }


        // Call the first_run function with the file pointer
        first_run(file, &ic, &dc, assembly_lines_array, label_table, file_number);
        assembly_lines_array->ic = ic;
        assembly_lines_array->dc = dc; //TODO dont count .entry and .extern in the dc

        //test_all_run(assembly_lines_array);
        //call the second_run function with the LinesArray table

        // Close the file
        fclose(file);




        start_second_run(assembly_lines_array);
        //printf("end %d file\n", file_number);

        create_ob_file(assembly_lines_array, 1);
        free_lines_array(assembly_lines_array);
    }

    //return EXIT_SUCCESS;
    return 1;
}
