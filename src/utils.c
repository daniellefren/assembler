#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/structs.h"


void lower_string(char *string){
// Convert command_name to lowercase
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }
}
