//
// Created by Royi David on 13/08/2024.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#include "../include/first_run.h"
#include "../include/second_run.h"
#include "../include/test_first_run.h"
#include "../include/errors.h"
#include "../include/files_handler.h"
#include "../Tests/output_test.h"
void run_assembler_on_file(LinesArray *assembly_lines_array, SymbolTable *symbol_table, char* filename, int file_number);
int main(int argc, char *argv[]);
#endif //ASSEMBLER_ASSEMBLER_H
