
#ifndef ASSEMBLER_FILES_HANDLER_H
#define ASSEMBLER_FILES_HANDLER_H
#include "../include/utils.h"
void create_output_files();
void add_extern_to_externals_file(Label *label, int file_number, int *ic);
void add_entry_to_entries_file(char *label, int file_number, int label_address);

#endif //ASSEMBLER_FILES_HANDLER_H
