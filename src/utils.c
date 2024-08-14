#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/utils.h"
#include "../include/errors.h"

void lower_string(char *string){
// Convert command_name to lowercase
    int i;
    for (i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}

char *trim_spaces(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void string_append(const char *first_string, const char *second_string, char *appended_string, size_t appended_string_size) {
    size_t first_string_size = strlen(first_string);
    size_t second_string_size = strlen(second_string);
    if (first_string_size + second_string_size + 1 > appended_string_size) {
        print_internal_error(ERROR_CODE_40, "");
        exit(EXIT_FAILURE);
    }

    // Copy first_string and append second_string
    strcpy(appended_string, first_string);
    strcat(appended_string, second_string);
}


void print_command(Command *command) {
    printf("Command name is %s\n", command->command_name);

    if (command->operand_number > 0) {
        printf("Src Operand value is %s, its type is %u and the classification type is %d\n",
               command->src_operand->value, command->src_operand->type, command->src_operand->classification_type);
    } else {
        printf("No Operands :(\n");
    }

    if (command->operand_number > 1) {
        printf("Dst Operand value is %s, its type is %u and the classification type is %d\n",
               command->dst_operand->value, command->dst_operand->type, command->dst_operand->classification_type);
    }
}


void print_directive(Directive *directive) {
    int i;

    if (is_directive_data(directive)) {
        if (directive->data_values_count > 0) {
            printf("The length of the directive is %zu and the values are:", directive->data_values_count);
            for (i = 0; i < directive->data_values_count; ++i) {
                if (directive->value != NULL) {
                    printf(" %s", directive->value[i]);
                }
            }
            printf("\n");
        } else {
            printf("The directive length is 0 and has no values\n");
        }
    } else {
        if (directive->value != NULL) {
            printf("The length of the directive is %zu and the value is %s\n",
                   directive->data_values_count, directive->value[0]);
        } else {
            print_internal_error(ERROR_CODE_41, "");
        }
    }
}


void print_instruction_line(InstructionLine *instructionLine) {
    printf("The Content of line is %s\n", instructionLine->line_content);
    printf("The Address of line is %d\n", instructionLine->starting_address);

    if (instructionLine->command != NULL) {
        print_command(instructionLine->command);
    } else if (instructionLine->directive != NULL) {
        print_directive(instructionLine->directive);
    }
}




void erase_file_data(const char *filename) {
    FILE *file = fopen(filename, "w"); // Open the file in write mode
    if (file == NULL) {
        print_internal_error(ERROR_CODE_48, filename);
        exit(EXIT_FAILURE);
    }
    fclose(file); // Closing the file truncates it to zero length
}

void extract_numbers(char *input, int length) {
    char *q;
    const char *p = input;
    char *numbers;

    // Allocate memory for the new string
    numbers = (char *)malloc((length + 1) * sizeof(char));
    if (numbers == NULL) {
        print_internal_error(ERROR_CODE_9, "extract_numbers");
        exit(EXIT_FAILURE);
    }
    q = numbers;


    while (*p != '\0') {
        if (isdigit((unsigned char)*p)) {
            *q++ = *p++;
        } else if (*p == '-' && isdigit((unsigned char)*(p + 1))) {
            *q++ = *p++; // Add the negative sign and move to the next character
            while (isdigit((unsigned char)*p)) {
                *q++ = *p++;
            }
        } else {
            p++;
        }
    }

    *q = '\0'; // Null-terminate the new string
    strcpy(input, numbers);

    free(numbers);
}


int char_to_int(char *str) {
    int sign = 1;
    int num = 0;

    // Check for optional negative sign
    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Convert string to integer
    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        str++;
    }

    return num * sign;
}

void extract_word_after_keyword(const char *input, char *output, const char *keyword) {
    const char *p = strstr(input, keyword);

    if (p != NULL) {
        p += strlen(keyword); // Move the pointer past the keyword
        while (isspace((unsigned char)*p)) {
            p++; // Skip any leading whitespace
        }
        // Copy the word after the keyword to the output buffer
        while (*p != '\0' && !isspace((unsigned char)*p)) {
            *output++ = *p++;
        }
        *output = '\0'; // Null-terminate the output string
    } else {
        output[0] = '\0'; // If the keyword is not found, output is an empty string
    }
}

void remove_first_character(char *str) {
    if (str != NULL && strlen(str) > 0) {
        memmove(str, str + 1, strlen(str));
    }
}

void add_number_to_string(char *buffer, int buffer_size, const char* source, int number) {
    snprintf(buffer, buffer_size, source, number);
}

int is_directive_data(Directive *directive) {
    if (directive->type == DATA) {
        printf("The directive is a data directive!\n");
        return 1;
    }
    return 0;
}

int is_directive_string(Directive *directive) {
    if (directive->type == STRING) {
        printf("The directive is a string directive!\n");
        return 1;
    }
    return 0;
}

void fill_octal_string_from_binary(const char *binary_string, int number_of_binary_bits, int offset, char *octal_string) {
    char octal_value[6] = {0};  // Buffer for octal value (assuming up to 4 octal digits)
    char extracted_binary[17] = {0};  // Buffer for extracted binary portion (up to 16 bits)
    printf("Number of binary bits is %d, offset is %d, ocatal string is %s and binary string is %s\n",number_of_binary_bits,offset,octal_string,binary_string);
    // Check for invalid input
    if (number_of_binary_bits <= 0 || offset < 0 || offset + number_of_binary_bits > strlen(binary_string)) {
        print_internal_error(ERROR_CODE_42, "");
        printf("\n\n\n\n");
        return;
    }

    // Ensure the number of bits is a multiple of 3
    if (number_of_binary_bits % 3 != 0) {
        print_internal_error(ERROR_CODE_43, int_to_string(number_of_binary_bits));
        return;
    }

    // Extract the desired portion of the binary string
    strncpy(extracted_binary, binary_string + offset, number_of_binary_bits);
    extracted_binary[number_of_binary_bits] = '\0';  // Null-terminate the extracted portion
    printf("Extracted binary string: %s\n", extracted_binary);

    // Convert binary to octal
    binary_to_octal(extracted_binary, octal_value);

    printf("Converted octal value: %s\n", octal_value);

    // Copy the octal value to the output string
    strcpy(octal_string, octal_value);
    printf("Octal string after strcpy: %s\n", octal_string);
}


void binary_to_octal(const char *binary_string, char *octal_string) {
    int i;
    int value;

    if (binary_string == NULL || strlen(binary_string) != BINARY_WORD_LENGTH) {
        print_internal_error(ERROR_CODE_44, "");
        return;
    }

    value = 0;

    // Initialize octal_string with '0's and null-terminate
    memset(octal_string, '0', OCTAL_LENGTH);
    octal_string[OCTAL_LENGTH] = '\0';

    // Convert binary string to decimal
    for (i = 0; i < BINARY_WORD_LENGTH; i++) {
        value = (value << 1) | (binary_string[i] - '0');
    }

    // Convert decimal to octal
    for (i = OCTAL_LENGTH - 1; i >= 0; i--) {
        octal_string[i] = (value & 7) + '0';  // Extract the last 3 bits and convert to octal
        value >>= 3; // Shift right by 3 bits (1 octal digit)
    }
}

int check_if_valid_integer(char *str) {
    int i = 0;
    int length = 0;

    // Check for an optional negative sign
    if (str[i] == '-') {
        i++;
    }

    // Ensure that the string contains at least one digit
    if (str[i] == '\0') {
        return 0;  // No digits found, invalid integer
    }

    // Check each character to ensure it's a digit
    for (; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;  // Invalid character found
        }
        length++;
    }

    return length;  // All checks passed, valid integer
}

void int_to_binary_string(int num, char *binary_string, int offset, int num_bits) {
    int i;

    // Handle negative numbers using two's complement
    if (num < 0) {
        num = (1 << num_bits) + num;  // Convert negative num to its two's complement
    }

    // Convert to binary representation
    for (i = 0; i < num_bits; i++) {
        binary_string[offset + num_bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
}


void char_to_binary_string(char c, char *binary_string, int offset, int num_bits) {
    int i;
    unsigned char uc;
    // Ensure binary_string is large enough
    if (binary_string == NULL || offset + num_bits > sizeof(char) * strlen(binary_string)) {
        print_internal_error(ERROR_CODE_45, "");
        return;
    }

    // Convert character to binary representation
    uc = (unsigned char)c;

    for (i = offset + num_bits - 1; i >= offset; i--) {
        binary_string[i] = (uc & 1) + '0';
        uc >>= 1;
    }
}

char* skip_spaces(char *line) {
    if (line == NULL) return NULL;
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

int ignore_line(char *line) {
    line = skip_spaces(line);
    return (*line == ';' || *line == '\0' || *line == '\n');
}

char* int_to_string(int number) {
    char *str = (char *)malloc(20 * sizeof(char)); // Allocate memory on the heap
    if (str == NULL) {
        print_internal_error(ERROR_CODE_9, "int_to_string");
        exit(EXIT_FAILURE);
    }
    sprintf(str, "%d", number);
    return str;
}


void print_current_directory(){
    char *cwd;
    size_t size = 1024;  // Initial buffer size

    // Allocate memory for the buffer
    cwd = (char *)malloc(size * sizeof(char));
    if (cwd == NULL) {
        print_internal_error(ERROR_CODE_9, "");
        exit(EXIT_FAILURE);
    }

    // Get the current working directory
    if (getcwd(cwd, size) == NULL) {
        perror("getcwd error");
        free(cwd);
        return;
    }

    // Print the current working directory
    printf("Current working directory: %s\n", cwd);

    // Free the allocated buffer
    free(cwd);
}

void strip_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}
