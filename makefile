CC = gcc  # Define compiler
CFLAGS = -g -ansi -Wall -Wno-comment -pedantic  # Define compilation flags

# Define the header files
HEADERS = assembler.h first_run.h prototypes.h second_run.h structs.h utils.h constants.h

# Define the source files (replace with your actual file names)
SRC_FILES = main.c first_run.c second_run.c utils.c

# Define the object files (automatically generated from source files)
OBJ_FILES = $(SRC_FILES:.c=.o)  # Pattern rule for object files

# Define the executable name
EXEC = assembler

# Target for building the executable
all: $(EXEC)

# Pattern rule for compiling C files
.PHONY: all clean

%.o: %.c $(HEADERS)  # Pattern for object files from C source
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for linking the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -lm  # Link with math library

# Rule for cleaning up object files
clean:
		-rm $(OBJ_FILES)
