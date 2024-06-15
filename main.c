#include <stdio.h>
#include <stdlib.h>

#include "first_run.h"

enum Command_Types {
    OPCODE = 1,
    DATA = 2,
    EXTERN = 3,
    STRING = 4,
    ENTRY = 5
};

const char *classification_types[] = {
        "immediate", "direct", "direct_register", "direct_register_addressing", "indirect_register_addressing"
};


int main(int argc, char *argv[]) {
    FILE *file;
    int ic, dc = 0

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
    first_run(file, &ic, &dc);

    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}