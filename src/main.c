#include <stdlib.h>
#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"

//TODO - handle ic and dc
// TODO - Add function that defines how many lines there is in an assembly command
int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0;
    int return_value = 0;


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
    return_value = first_run(file, &ic, &dc, assembly_lines_array, label_table);
    if(return_value){
        printf("noooooo");
        return -1  ;
    }
    //call the second_run function with the LinesArray table


    printf("Get all instructions:\n");
   /*
    for(int k=0; k<2;k++){
        printf("line - \n");

        printf("%s \n", assembly_lines_array.lines[k].line_content);
        printf("first_operand - \n");
        printf("%s \n", assembly_lines_array.lines[k].first_operand);
        printf("second_operand - \n");
        printf("%s \n", assembly_lines_array.lines[k].second_operand);

    }

    */
    // Close the file
    fclose(file);
    printf("\nYessss\n");
    start_second_run(assembly_lines_array);
    //test_all_run(assembly_lines_array);





    //return EXIT_SUCCESS;
    return 1;
}
