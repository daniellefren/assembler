#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/constants.h"



void test_all_run(LinesArray* assembly_lines_array){
    printf("\nstarting final check\n");

    for(int i=0;i<assembly_lines_array->number_of_line;i++){
        printf("%d line number\n", i);
        InstructionLine *line = assembly_lines_array->lines[i];
        printf("line %s\n", line->line_content);

        if(line->is_symbol){
            Symbol *symbol = line->symbol;
            printf("symbol name %s\n", symbol->name);
            size_t length = strlen(symbol->name);
        }
        if(line->instruction_type == COMMAND){
            Command *command = line->command;
            printf("command %s\n", command->command_name);
            switch (command->operand_number) {
                case 0: {
                    printf("no operands\n");
                    break;
                }
                case 1: {
                    Operand *operand = command->src_operand;
                    printf("Only operand %s\n", operand->value);
                    break;
                }
                case 2: {
                    Operand *operand_a = command->src_operand;
                    printf("First operand %s\n", operand_a->value);
                    printf("first operand type %d", operand_a->type);
                    Operand *operand_b = command->dst_operand;
                    printf("Second operand %s\n", operand_b->value);
                    printf("Second operand type %d\n", operand_b->type);

                    break;
                }
            }
        }
        if(line->instruction_type == DATA_DIRECTIVE){
            Directive *directive = line->directive;
            printf("directive value: %d\n", directive->type);
            for(int j=0;j<directive->data_values_count;j++){
                printf("%s,", directive->value[j]);
            }
            printf("\n");
        }
    }
}