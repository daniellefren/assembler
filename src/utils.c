#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../include/utils.h"



void lower_string(char *string){
// Convert command_name to lowercase
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}

// Function to trim leading and trailing spaces
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

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

void string_append(const char *first_string, const char *second_String, char *appended_string, size_t appended_string_size) {
    /*
     In calling function
    char appended_string[100];  // Pre-allocate memory for combined string
    string_append(string1, string2, appended_string, sizeof(appended_string_size));
    */
    size_t first_string_size = strlen(first_string);
    size_t second_string_size = strlen(second_String);
    if (first_string_size + second_string_size + 1 > appended_string_size) {
        // Handle insufficient space in destination (e.g., return error code)
        return;
    }

    // Copy s1 and s2 using strcpy and strcat
    strcpy(appended_string, first_string);
    strcat(appended_string, second_String);
}

void print_command(Command *command){
    printf("Command name is %s\n", command->command_name);
    if (command->operand_number > 0){
        printf("Src Operand value is %s, it's type is %u and the classification type is %d\n", command->src_operand->value, command->src_operand->type, command->src_operand->classification_type);
    }
    else{
        printf("No Operands :(\n");
    }
    if (command->operand_number > 1){
        printf("Dst Operand value is %s, it's type is %u and the classification type is %d\n", command->dst_operand->value, command->dst_operand->type, command->dst_operand->classification_type);
    }

}

void print_directive(Directive *directive){
    if (is_directive_data(directive)){
        if (directive->data_values_count){
            printf("The Lenth of the directive is %zu and the values are", directive->data_values_count);
        }

        for (int i = 0; i < directive->data_values_count; ++i) {
            printf(" %s", directive->value[i]);
        }
        printf("\n");
    }
    else{
        if (directive->value != NULL){
            printf("The Lenth of the directive is %zu and the value is %s\n", directive->data_values_count, directive->value[0]);
        }
        else{
            fprintf(stderr, "Error: there is no values in the directive\n");
            return;
        }

    }

}

void print_instruction_line(InstructionLine *instructionLine){
    printf("The Content of line is %s\n", instructionLine->line_content);
    printf("The Address of line is %d\n", instructionLine->starting_address);
    if (instructionLine->command != NULL){
        print_command(instructionLine->command);
    }
    else if (instructionLine->directive != NULL){
        print_directive(instructionLine->directive);
    }
}




void erase_file_data(const char *filename) {
    FILE *file = fopen(filename, "w"); // Open the file in write mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fclose(file); // Closing the file truncates it to zero length
}

// Function to extract numeric characters, including negative numbers, from a string
char* extract_numbers(const char *input, int length) {
    // Allocate memory for the new string
    char *numbers = (char *)malloc(length + 1);
    if (numbers == NULL) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Extract the numeric characters
    char *q = numbers;
    const char *p = input;

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

    return numbers;
}

int char_to_int(char *str) {
    int sign = 1;
    int num = 0;

    if (*str == '-') {
        sign = -1;
        str++;
    }

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

// Function to remove the first character from a string
void remove_first_character(char *str) {
    if (str != NULL && strlen(str) > 0) {
        memmove(str, str + 1, strlen(str));
    }
}

void add_number_to_string(char *buffer, const char* source, size_t buffer_size, int number) {
    snprintf(buffer, buffer_size, source, number);
}


bool is_directive_data(Directive *directive){
    if (directive->type == DATA){
        printf("The directive is data directive!\n");
        return true;
    }
    return false;
}

bool is_directive_string(Directive *directive){
    if (directive->type == STRING){
        printf("The directive is string directive!\n");
        return true;
    }
    return false;
}

void fill_octal_string_from_binary(const char *binary_string, int number_of_binary_bits, int offset, char *octal_string) {
    char octal_value[6] = {0};  // Initialize and ensure it's null-terminated
    char extracted_binary[17] = {0};  // Assuming maximum 15 bits + null terminator

    // Check for invalid input
    if (number_of_binary_bits <= 0 || offset < 0 || offset + number_of_binary_bits > strlen(binary_string)) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    // Ensure the number of bits is a multiple of 3
    if (number_of_binary_bits % 3 != 0) {
        fprintf(stderr, "Number of bits must be a multiple of 3\n");
        return;
    }

    // Extract the desired portion
    strncpy(extracted_binary, binary_string + offset, number_of_binary_bits);
    extracted_binary[number_of_binary_bits] = '\0';  // Ensure it's null-terminated
    printf("Extracted binary string: %s\n", extracted_binary);

    // Convert binary to octal
    binary_to_octal(extracted_binary, octal_value);

    printf("Converted octal value: %s\n", octal_value);

    // Copy the octal value to the output string
    strcpy(octal_string, octal_value);
    printf("Octal string after strcpy: %s\n", octal_string);
}


void binary_to_octal(const char *binary_string, char *octal_string) {
    if (binary_string == NULL || strlen(binary_string) != BINARY_WORD_LENGTH) {
        fprintf(stderr, "Invalid binary string\n");
        return;
    }
    int i, j, value = 0;
    // Initialize octal_string with '0's
    memset(octal_string, '0', OCTAL_LENGTH);
    octal_string[OCTAL_LENGTH] = '\0';
    // Convert binary to decimal
    for (i = 0; i < BINARY_WORD_LENGTH; i++) {
        value = (value << 1) | (binary_string[i] - '0');
    }
    // Convert decimal to octal
    for (i = OCTAL_LENGTH - 1; i >= 0; i--) {
        octal_string[i] = (value & 7) + '0';  // Get the last 3 bits and convert to octal digit
        value >>= 3; // Shift right by 3 bits (1 octal digit)
    }
}

/* Check if given string is integer
 Return 0 if no or the length if yes */
int check_if_valid_integer(char *str) {
    printf("strrrr %s", str);
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

    // Handle negative numbers
    if (num < 0) {
        binary_string[offset] = '1';
        num = -num;
    } else {
        binary_string[offset] = '0';
    }

    // Convert to binary
    for (i = 0; i < num_bits; i++) {
        binary_string[offset + num_bits - i - 1] = (num & (1 << i)) ? '1' : '0';
    }
}

void char_to_binary_string(char c, char *binary_string, int offset, int num_bits) {
    int i;

    // Ensure binary_string is large enough to hold the result
    if (binary_string == NULL || offset + num_bits > CHAR_BIT * sizeof(char) * strlen(binary_string)) {
        fprintf(stderr, "Invalid binary string buffer\n");
        return;
    }
    // Convert character to unsigned char for positive representation
    unsigned char uc = (unsigned char) c;
    // Convert to binary
    for (i = offset + num_bits - 1; i >= offset; i--) {
        binary_string[i] = (uc & 1) + '0';
        uc >>= 1;
    }
}