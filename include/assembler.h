#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#include "first_run.h"
#include "second_run.h"
#include "../Tests/output_test.h"
#include "structs.h"


/**
 * Run the assembler on a given assembly source file.
 *
 * This function performs the following steps:
 * 1. Initializes the LinesArray and SymbolTable structures.
 * 2. Opens the specified assembly source file for reading.
 * 3. Executes the first run of the assembler to parse the file and populate the LinesArray and SymbolTable.
 * 6. Executes the second run of the assembler to generate the final output files (e.g., object file, externals, entries).
 * 7. Frees the memory allocated for the LinesArray and SymbolTable.
 *
 * @param filename - The name of the assembly source file to be processed.
 * @param file_number - An identifier number used to generate output file names.
 */
void run_assembler_on_file(char* filename, int file_number);

#endif
