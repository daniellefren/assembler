#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <first_run.c>


int main(){
    file = fopen("assembly_file.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    first_run(file)

}

typedef struct {
    char *instruction; // String containing the assembly instruction
    // Add more fields if needed, like operand types, etc.
} InstructionLine;



InstructionLine* read_assembly_file(char *filename, int *num_lines) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL; // Handle file opening error
    }

    int line_count = 0;
    size_t line_size = 100; // Adjust based on expected instruction length
    char *line_buffer = (char *)malloc(line_size * sizeof(char));

    InstructionLine *instructions = NULL;

    while (fgets(line_buffer, line_size, fp) != NULL) {
        // Remove trailing newline character
        line_buffer[strcspn(line_buffer, "\n")] = '\0';

        // Allocate memory for a new instruction line
        instructions = (InstructionLine *)realloc(instructions, (line_count + 1) * sizeof(InstructionLine));
        instructions[line_count].instruction = (char *)malloc(line_size * sizeof(char));
        strcpy(instructions[line_count].instruction, line_buffer);

        line_count++;
    }

    free(line_buffer);
    fclose(fp);

    *num_lines = line_count;
    return instructions;
}

void simulate_binary_generation(InstructionLine *instructions, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        printf("Line %d: Instruction - %s\n", i + 1, instructions[i].instruction);
        // Replace with actual logic to convert instruction to binary representation
        printf("  Simulated Binary: (placeholder - actual binary generation needed)\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    int num_lines;
    InstructionLine *instructions = read_assembly_file(argv[1], &num_lines);

    if (instructions == NULL) {
        printf("Error reading assembly file.\n");
        return 1;
    }

    simulate_binary_generation(instructions, num_lines);

    // Free memory allocated for instructions
    for (int i = 0; i < num_lines; i++) {
        free(instructions[i].instruction);
    }
    free(instructions);

    return 0;
}