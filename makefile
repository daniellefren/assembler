# Define compiler
CC = gcc

# Define compilation flags (feel free to customize)
CFLAGS = -g -ansi -Wall -Wno-comment -pedantic

# Define the header files directory
HEADERS_DIR = include

# Define all header files (relative to HEADERS_DIR)
HEADERS = $(shell find $(HEADERS_DIR) -type f -name "*.h")

# Define the source directory
SRC_DIR = src

# Define all source files (relative to SRC_DIR)
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")

# Define the object files (automatically generated from source files)
OBJ_FILES = $(SRC_FILES:.c=.o)  # Pattern rule for object files

# Define the executable name
EXEC = assembler

# Target for building the executable
all: $(EXEC)

# Pattern rule for compiling C files (using SRC_DIR)
.PHONY: all clean

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS_DIR)/*.h  # Pattern for object files from C source with SRC_DIR
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for linking the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -lm  # Link with math library

# Rule for cleaning up object files
clean:
	-rm $(OBJ_FILES)
