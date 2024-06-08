#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "prototypes.h"
#include "utils.h"

void first_run(*file){
    while (fgets(line, sizeof(line), file)) {
    // Print the line to the standard output
    printf("%s", line);
}
}