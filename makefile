# Define compiler
CC = gcc

# Define compilation flags (feel free to customize)
CFLAGS = -g -ansi -Wall -Wno-comment -pedantic

# Define the header files directories
HEADERS_DIR = include
EXTRA_HEADERS_DIR = Tests

# Define all header files (relative to HEADERS_DIR and EXTRA_HEADERS_DIR)
HEADERS = $(shell find $(HEADERS_DIR) -type f -name "*.h")
EXTRA_HEADERS = $(shell find $(EXTRA_HEADERS_DIR) -type f -name "*.h")

# Define the source directories
SRC_DIR = src
EXTRA_SRC_DIR = Tests

# Define all source files (relative to SRC_DIR and EXTRA_SRC_DIR)
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
EXTRA_SRC_FILES = $(shell find $(EXTRA_SRC_DIR) -type f -name "*.c")

# Define the object files (automatically generated from source files)
OBJ_FILES = $(SRC_FILES:.c=.o)
EXTRA_OBJ_FILES = $(EXTRA_SRC_FILES:.c=.o)  # Pattern rule for object files in EXTRA_SRC_DIR

# Define the executable name
EXEC = assembler

# Target for building the executable
all: $(EXEC)

# Pattern rule for compiling C files (using SRC_DIR and EXTRA_SRC_DIR)
.PHONY: all clean

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) $(EXTRA_HEADERS)  # Pattern for object files from SRC_DIR
	$(CC) $(CFLAGS) -c $< -o $@

$(EXTRA_SRC_DIR)/%.o: $(EXTRA_SRC_DIR)/%.c $(HEADERS) $(EXTRA_HEADERS)  # Pattern for object files from EXTRA_SRC_DIR
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for linking the executable
$(EXEC): $(OBJ_FILES) $(EXTRA_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(EXTRA_OBJ_FILES) -lm  # Link with math library

# Rule for cleaning up object files
clean:
	-rm $(OBJ_FILES) $(EXTRA_OBJ_FILES)
