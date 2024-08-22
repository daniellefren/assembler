#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "../include/files_handler.h"


void create_ob_file(LinesArray *linesArray, char* file_name){
    char ob_file_name[MAX_FILE_NAME_LEN];
    char ob_file_name_with_directive[MAX_FILE_NAME_LEN];
    int number_of_command;
    int number_of_directive;
    FILE *object_file;
    printf("\nStarting to create object file\n");

    if (linesArray == NULL){
        print_internal_error(ERROR_CODE_28, "");
        exit(EXIT_FAILURE);
    }

    get_output_filename(ob_file_name, ob_file_name_with_directive, OBJECT_FILE_EXTENSION, file_name);

    object_file = open_ob_file(ob_file_name_with_directive);
    number_of_command = linesArray->ic - STARTING_IC;
    number_of_directive = linesArray->dc;

    add_first_line_to_ob_file(number_of_command, number_of_directive, object_file);
    add_all_command_lines_to_ob_file(linesArray, object_file);
    add_all_directive_lines_to_ob_file(linesArray, object_file);
    printf("Finished creating object file!\n");
    fclose(object_file);
}

FILE* open_ob_file(char *ob_file_name) {
    FILE *file;

    /* Attempt to open the file in write mode ("w" will create a new file or overwrite an existing file) */
    file = fopen(ob_file_name, "w");
    if (file == NULL) {
        print_internal_error(ERROR_CODE_48, ob_file_name);
        exit(EXIT_FAILURE);
    }
    return file;
}


void add_first_line_to_ob_file(int number_of_command, int number_of_directive, FILE *object_file) {
    if (object_file == NULL) {
        print_internal_error(ERROR_CODE_54, "");
        exit(EXIT_FAILURE);
    }

    /* Write the number of commands and directives to the first line of the file */
    fprintf(object_file, "  %d %d\n", number_of_command, number_of_directive);
}


void add_all_command_lines_to_ob_file(LinesArray *lines_array, FILE *object_file){
    int i;
    InstructionLine *p_line;
    /* Iterate over all lines in the lines array */
    for (i = 0; i < lines_array->number_of_line; ++i) {
        p_line = lines_array->lines[i];
        if (p_line == NULL) {
            print_internal_error(ERROR_CODE_29, int_to_string(i));
            continue;
        }

        if (p_line->instruction_type == COMMAND) {
            add_command_line_to_ob_file(p_line, object_file);
        }
    }
}


void add_all_directive_lines_to_ob_file(LinesArray *lines_array, FILE *object_file){
    int i;
    InstructionLine *p_line;
    /* Iterate over all lines in the lines array */
    for (i = 0; i < lines_array->number_of_line; ++i) {
        p_line = lines_array->lines[i];
        if (p_line == NULL) {
            print_internal_error(ERROR_CODE_30, int_to_string(i));
            continue;
        }

        if (lines_array->lines[i]->instruction_type == DATA_DIRECTIVE) {
           add_directive_line_to_ob_file(p_line, object_file);
        }
    }
}



void add_command_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    int i;
    char *octal_number;            /* Pointer to hold the octal string representation of the binary instruction */
    int instruction_address;       /* Variable to store the starting address of the instruction */

    /* Allocate memory for the octal number string (5 digits + null terminator) */
    octal_number = (char *)malloc((OCTAL_LENGTH + 1) * sizeof(char));
    if (octal_number == NULL) {
        print_internal_error(ERROR_CODE_55, "");
        exit(EXIT_FAILURE);
    }

    instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != COMMAND){
        print_internal_error(ERROR_CODE_57, "");
        exit(EXIT_FAILURE);
    }
    /* Loop through each binary line in the instruction */
    for (i = 0; i < instructionLine->binary_line_count; ++i) {

        /* Convert the binary instruction to an octal string */
        fill_octal_string_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH, octal_number);
        if ((instruction_address < 1000) && (instruction_address > 99)){ /*IC start from 100 so no need to add more then one 0 at the starts */
            fprintf(object_file, "0%d %s\n", instruction_address, octal_number);
        }
        else {
            fprintf(object_file, "%d %s\n", instruction_address, octal_number);
        }

        instruction_address += 1;
    }
    free(octal_number);
}

void add_directive_line_to_ob_file(InstructionLine *instructionLine, FILE *object_file){
    int i;
    char *octal_number;            /* Pointer to hold the octal string representation of the binary instruction */
    int instruction_address;       /* Variable to store the starting address of the instruction */
    char *padded_instruction_address = malloc(sizeof(char) *5);

    /* Allocate memory for the octal number string (5 digits + null terminator) */
    octal_number = (char *)malloc((OCTAL_LENGTH + 1) * sizeof(char));
    if (octal_number == NULL) {
        print_internal_error(ERROR_CODE_55, "");
        exit(EXIT_FAILURE);
    }

    instruction_address = instructionLine->starting_address;
    if (instructionLine->instruction_type != DATA_DIRECTIVE){
        print_internal_error(ERROR_CODE_57, "");
        exit(EXIT_FAILURE);
    }

    /* Loop through each binary line in the instruction */
    for (i = 0; i < instructionLine->binary_line_count; ++i) {
        /* Convert the binary instruction to an octal string */
        fill_octal_string_from_binary(instructionLine->binary_instruction, BINARY_WORD_LENGTH, i * BINARY_WORD_LENGTH, octal_number);
        strcpy(padded_instruction_address, pad_address(instruction_address));

        fprintf(object_file, "%s %s\n", padded_instruction_address, octal_number);
        instruction_address += 1;
    }
}

void add_entry_to_entries_file(char *symbol_name, char* file_name, int symbol_address){
    char *new_ic;
    char entries_file_name[MAX_FILE_NAME_LEN];
    char entries_file_name_and_directive[MAX_FILE_NAME_LEN];
    FILE *file;
    get_output_filename(entries_file_name, entries_file_name_and_directive, ENTRIES_FILE_EXTENSION, file_name);

    file = fopen(entries_file_name_and_directive, "a");
    if (file == NULL) {
        print_internal_error(ERROR_CODE_48, entries_file_name_and_directive);
        exit(EXIT_FAILURE);
    }
    if(search_in_file(entries_file_name_and_directive, symbol_name)){
        return;
    }

    new_ic = malloc(sizeof(char) *5);
    strcpy(new_ic, pad_address(symbol_address));

    fprintf(file, "%s %s\n", symbol_name, new_ic);
    free(new_ic);
    fclose(file);
}



void add_extern_to_externals_file(char *symbol_name, char *file_name, int ic){
    FILE *file;
    char externals_file_name[MAX_FILE_NAME_LEN];
    char externals_file_name_and_directive[MAX_FILE_NAME_LEN];
    char *new_ic = malloc(sizeof(char) *5);

    get_output_filename(externals_file_name, externals_file_name_and_directive, EXTERNALS_FILE_EXTENSION, file_name);

    strcpy(new_ic, pad_address(ic));

    if(search_in_file(externals_file_name_and_directive, new_ic)){
        return;
    }

    file = fopen(externals_file_name_and_directive, "a");
    if (file == NULL) {
        print_internal_error(ERROR_CODE_48, externals_file_name_and_directive);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s %s\n", symbol_name, new_ic);
    free(new_ic);
    fclose(file);
}

void get_output_filename(char *basic_filename, char* final_file_name, const char* extension, char* src_file_name){
    strcpy(basic_filename, src_file_name);
    strcpy(final_file_name, OUTPUT_DIRECTORY_NAME);

    replace_extension(basic_filename, extension);
    strcat(final_file_name, "/");
    strcat(final_file_name, basic_filename);
}

void get_input_filename(char* file_name, char* given_file_name){
    strcpy(file_name, INPUT_DIRECTORY_NAME);
    strcat(file_name, "/");
    strcat(file_name, given_file_name);
    strcat(file_name, ".");
    strcat(file_name, SRC_FILE_NAME_EXTENSION);
}

void replace_extension(char *file_path, const char *new_extension) {
    /* Find the last occurrence of '.' in the file path */
    char *dot_position = strrchr(file_path, '.');

    if (dot_position != NULL) {
        /* Replace everything after the last dot with the new extension */
        strcpy(dot_position + 1, new_extension);
    } else {
        /* If no dot is found, you may want to handle it (e.g., add the extension) */
        strcat(file_path, ".");
        strcat(file_path, new_extension);
    }
}

char *pad_address(int address){
    char *new_address = malloc(sizeof(char) *5);
    if ((address < 1000) && (address >= 99)){
        new_address[0] = '0';
        strcpy(new_address+1, int_to_string(address));
    }
    else{
        strcpy(new_address, int_to_string(address));
    }
    return new_address;
}

int write_line_to_file(char *line, char* new_file_name) {
    FILE *outputFile = fopen(new_file_name, "a"); /* Open in append mode */
    if (!outputFile) {
        print_internal_error(ERROR_CODE_48, "write_line_to_file");

        print_internal_error(ERROR_CODE_48, new_file_name);
        exit(EXIT_FAILURE);
    }
    /* Write the string to the file */
    fprintf(outputFile, "%s\n", line);

    fclose(outputFile);
    return 0;  /* Indicate success */
}

void write_expanded_macros_to_file(MacroTable *macro_table, char* new_file_name) {
    int i;
    int j;
    FILE *outputFile = fopen(new_file_name, "a"); /* Open in append mode */
    if (!outputFile) {
        print_internal_error(ERROR_CODE_49, new_file_name);
        exit(EXIT_FAILURE);
    }

    /* Write each macro's body to the file */
    for (i = 0; i < macro_table->count; ++i) {
        for (j = 0; j < macro_table->macros[i]->lineCount; ++j) {
            fprintf(outputFile, "%s\n", macro_table->macros[i]->body[j]);
        }
    }

    fclose(outputFile);
}

void add_output_directory(void) {
    const char *dir_name = OUTPUT_DIRECTORY_NAME;
    char *modifiable_dir_name;
    struct stat st = {0};

    modifiable_dir_name = malloc(strlen(dir_name) + 1);
    if (modifiable_dir_name == NULL) {
        /* Handle memory allocation failure */
        print_internal_error(ERROR_CODE_9, modifiable_dir_name);
        exit(EXIT_FAILURE);
    }
    strcpy(modifiable_dir_name, dir_name);

    /* Check if the directory exists */
    if (stat(modifiable_dir_name, &st) == -1) {
        /* Directory does not exist, create it */
        if (mkdir(modifiable_dir_name, 0755) == 1) {
            print_internal_error(ERROR_CODE_56, modifiable_dir_name);
            exit(EXIT_FAILURE);
        }
    }
}

FILE* open_file(char *file_name, char *mode) {
    FILE *file;

    /* Attempt to open the file */
    file = fopen(file_name, mode);
    if (file == NULL) {
        print_internal_error(ERROR_CODE_48, file_name);
        exit(EXIT_FAILURE);
    }
    return file;
}

int open_two_files_and_compare(char *file1_name, char *file2_name) {

    int result;
    FILE *file1;
    FILE *file2;

    if(file_exists(file1_name) == 0 && file_exists(file1_name) == 0){
        printf("Both files does not exist\n");
        return 1;
    }
    else if(file_exists(file1_name) == 0 || file_exists(file1_name) == 0){
        printf("One of the given files does not exist!\n");
        return 0;
    }

    file1 = open_file(file1_name, "r");
    file2 = open_file(file2_name, "r");

    result = compare_files(file1, file2); /*return 0 for different */
    fclose(file1);
    fclose(file2);


    if (result == 1) {
        printf("Files are identical.\n");
    } else {
        printf("Files are different.\n");
    }

    return result;
}


int search_in_file(char *filename, char *search_str) {
    FILE *file;
    char line[256];  /* Buffer to store each line from the file */
    int found;

    found = 0;


    file = fopen(filename, "r");
    if (file == NULL) {
        return 0; /* Means the file does not exist, and therefore the string does not exist in the file*/
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, search_str) != NULL) {
            found = 1;
            break;
        }
    }

    fclose(file);

    return found;
}

void delete_files_in_directory(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    char file_path[1024];
    size_t dir_path_len;

    dir = opendir(dir_path);
    if (dir == NULL) {
        print_internal_error(ERROR_CODE_52, (char *)dir_path);
        exit(EXIT_FAILURE);
    }

    dir_path_len = strlen(dir_path);

    /* Iterate over all files in the directory */
    while ((entry = readdir(dir)) != NULL) {
        /* Skip "." and ".." */
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        /* Construct the full path to the file */
        if (dir_path_len + 1 + strlen(entry->d_name) >= sizeof(file_path)) {
            print_internal_error(ERROR_CODE_53, "File path too long");
            continue;
        }

        strcpy(file_path, dir_path);
        strcat(file_path, "/");
        strcat(file_path, entry->d_name);

        /* Remove the file */
        if (remove(file_path) == 1) {
            print_internal_error(ERROR_CODE_53, file_path);
        }
    }

    closedir(dir);
}


int file_exists(char *filename) {
    return access(filename, F_OK) == 0;
}


int compare_files(FILE *file1, FILE *file2) {
    char line1[256]; /* Buffer to store lines from file1 */
    char line2[256]; /* Buffer to store lines from file2 */
    int line_number = 1; /* Line counter to keep track of the line being compared */

    /* Read lines from both files until end-of-file or error */
    /* Check if files are opened successfully*/
    if (file1 == NULL) {
        print_internal_error(ERROR_CODE_48, "file 1");
        exit(EXIT_FAILURE);
    }
    if (file2 == NULL) {
        print_internal_error(ERROR_CODE_48, "file 2");
        exit(EXIT_FAILURE);
    }

    /* Read the first lines from both files */
    if (fgets(line1, sizeof(line1), file1) == NULL) {
        print_internal_error(ERROR_CODE_62, "File1");
    }

    if (fgets(line2, sizeof(line2), file2) == NULL) {
        print_internal_error(ERROR_CODE_62, "File2");
    }

    while (fgets(line1, sizeof(line1), file1) != NULL &&
           fgets(line2, sizeof(line2), file2) != NULL) {
        strip_newline(line1);
        strip_newline(line2);
        /* Compare the lines from both files */
        if (strcmp(line1, line2) != 0) {
            /* If lines are different, print the line number and the differing lines */
            printf("Difference found at line %d:\n", line_number);
            printf("File1: %s\n", line1);
            printf("File2: %s\n", line2);
            return 0; /* Return 0 to indicate that the files are different */
        }
        line_number++; /* Increment the line number for the next comparison */
    }

    /* Check if one file ended before the other */
    if (fgets(line1, sizeof(line1), file1) != NULL || fgets(line2, sizeof(line2), file2) != NULL) {
        printf("Files have different lengths.\n");
        return 0; /* Return 0 to indicate that the files are different */
    }

    /* If we reach this point, files are identical Return 1 to indicate that the files are identical */
    return 1;
}

long get_file_size(FILE *file) {
    long size;
    /* Move the file pointer to the end of the file */
    if (fseek(file, 0, SEEK_END) == -1){
        print_internal_error(ERROR_CODE_60, "");
        exit(EXIT_FAILURE);
    }
    /* Get the current file pointer position, which is the size of the file */
    size = ftell(file);
    if (size == -1){
        print_internal_error(ERROR_CODE_61, "");
        exit(EXIT_FAILURE);
    }
    /* Move the file pointer back to the beginning of the file */
    if (fseek(file, 0, SEEK_SET) == -1){
        print_internal_error(ERROR_CODE_60, "");
        exit(EXIT_FAILURE);
    }
    return size;
}

char* get_filename(char *file_path) {
    /* Find the last occurrence of '/' in the file path */
    char *last_slash = strrchr(file_path, '/');

    /* If found, return the character after the last slash */
    if (last_slash != NULL) {
        return last_slash + 1;
    } else {
        /* If no slash is found, the entire path is the filename */
        return (char *)file_path;
    }
}
