#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <first_run.c>


int main(){
    FILE *file;
    file = fopen("assembly_file.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    first_run(file);

}