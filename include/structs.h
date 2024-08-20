#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H
#include "constants.h"


/**
 * Struct representing a directive in the assembly code.
 */
typedef struct {
    char symbol[MAX_SYMBOL_LENGTH];          /**< The symbol name associated with the directive. */
    enum directives type;                    /**< The type of directive (.data or .string or .extern ot .entry). */
    char **value;                            /**< Pointer to an array of strings representing the values of the directive. */
    size_t data_values_count;                /**< The number of data values associated with the directive. */
} Directive;

/**
 * Struct representing a symbol in the assembly code.
 */
typedef struct {
    char name[MAX_SYMBOL_LENGTH];            /**< The name of the symbol. */
    enum instruction_types type;             /**< The type of the symbol */
    int address;                             /**< The address of the symbol in the memory. */
    int is_extern;                           /**< Flag indicating if the symbol is external. */
    int is_entry;                            /**< Flag indicating if the symbol is an entry. */
} Symbol;

/**
 * Struct representing a symbol table.
 */
typedef struct {
    Symbol **symbols;                         /**< Pointer to an array of pointers to Symbol structs. */
    size_t size;                             /**< The current number of symbols in the table. */
    size_t capacity;                         /**< The maximum capacity of the symbol table. */
} SymbolTable;

/**
 * Struct representing an operand in an assembly command.
 */
typedef struct {
    char *value;                             /**< The value of the operand as a string. */
    enum operand_types type;                 /**< The type of operand (symbol, register, or number). */
    enum operand_classification_type classification_type; /**< The classification type of the operand. */
    Symbol *symbol;                          /**< Pointer to the Symbol struct associated with the operand, if applicable. */
} Operand;

/**
 * Struct representing a command in the assembly code.
 */
typedef struct {
    char *command_name;                      /**< The name of the command. */
    enum opcode_command opcode_command_type; /**< The type of opcode command. */
    unsigned int operand_number;             /**< The number of operands associated with the command. */
    Operand *src_operand;                    /**< Pointer to the source operand struct. */
    Operand *dst_operand;                    /**< Pointer to the destination operand struct. */
} Command;

/**
 * Struct representing an instruction line in the assembly code.
 */
typedef struct {
    char *line_content;                      /**< The content of the assembly instruction line. */
    size_t length;                           /**< The length of the line content (excluding the null terminator). */
    enum instruction_types instruction_type; /**< The type of instruction. */
    int is_symbol;                           /**< Flag indicating if the line contains a symbol. */
    Command *command;                        /**< Pointer to the Command struct associated with the line, if applicable. */
    Directive *directive;                    /**< Pointer to the Directive struct associated with the line, if applicable. */
    Symbol *symbol;                          /**< Pointer to the Symbol struct associated with the line, if applicable. */
    int file_number;                          /**< The file number of the src assembly file. */
    char *file_name;                          /**< The file name of the src assembly file. */
    int starting_address;                    /**< The starting address of the instruction in memory. */
    int binary_line_count;                   /**< The number of binary lines generated for the instruction. */
    char *binary_instruction;                /**< Pointer to the binary representation of the instruction. */
} InstructionLine;

/**
 * Struct representing an array of instruction lines.
 */
typedef struct {
    InstructionLine **lines;                  /**< Pointer to an array of pointers to InstructionLine structs. */
    unsigned int number_of_line;             /**< The number of lines currently stored in the array. */
    size_t capacity;                         /**< The maximum capacity of the lines array. */
    int ic;                                  /**< Instruction counter (IC), tracking the memory address for instructions. */
    int dc;                                  /**< Data counter (DC), tracking the memory address for data. */
} LinesArray;

/**
 * Struct representing a macro in the assembly code.
 */
typedef struct {
    char *name;            /**< The name of the macro. */
    char body[MAX_MACRO_LENGTH][MAX_LINE_LENGTH]; /**< The body of the macro, stored as an array of lines. */
    int lineCount;                           /**< The number of lines in the macro body. */
} Macro;

/**
 * Struct representing a table of macros.
 */
typedef struct {
    Macro **macros;                           /**< Pointer to an array of Macro structs. */
    int count;                               /**< The current number of macros stored in the table. */
    int capacity;                            /**< The maximum capacity of the macro table. */
} MacroTable;



/**
 * Free the memory allocated for a LinesArray structure.
 * @param lines_array_pointer - Pointer to the LinesArray structure to be freed.
 */
void free_lines_array(LinesArray *lines_array_pointer);

/**
 * Free the memory allocated for an InstructionLine structure.
 * @param instruction_line_pointer - Pointer to the InstructionLine structure to be freed.
 */
void free_instruction_line(InstructionLine *instruction_line_pointer);

/**
 * Free the memory allocated for a MacroTable structure.
 * @param macro_table - Pointer to the MacroTable structure to be freed.
 */
void free_macro_table(MacroTable *macro_table);

/**
 * Free the memory allocated for a SymbolTable structure.
 * @param symbol_table - Pointer to the SymbolTable structure to be freed.
 */
void free_symbol_table(SymbolTable *symbol_table);

/**
 * Frees the memory allocated for the binary instruction in an InstructionLine.
 *
 * @param p_line Pointer to the InstructionLine structure.
 */
void free_binary_instruction(InstructionLine *p_line);

/**
 * Add a new instruction line to the LinesArray.
 * The array is resized if needed.
 * @param lines_array - Pointer to the LinesArray structure.
 * @param instruction_line - Pointer to the InstructionLine structure to be added.
 */
void addInstructionLine(LinesArray *lines_array, InstructionLine *instruction_line);

/**
 * Initialize a LinesArray structure with a given initial capacity.
 * @param initial_capacity - The initial capacity of the array.
 * @return A pointer to the initialized LinesArray structure.
 */
LinesArray *init_lines_array(int initial_capacity);

/**
 * Initialize a SymbolTable structure with a given initial capacity.
 * @param initial_capacity - The initial capacity of the array.
 * @return A pointer to the initialized SymbolTable structure.
 */
SymbolTable * init_symbol_table(int initial_capacity);

/**
 * Initialize a MacroTable structure with a default capacity.
 * @param table - Pointer to the MacroTable structure to initialize.
 */
MacroTable *init_macro_table(void );

/**
 * Adds a new macro to the macro table, expanding the table if necessary.
 * @param macro_table A pointer to the `MacroTable` structure that stores the names and bodies of all macros encountered during the pre-run.
 * @param new_macro The `Macro` structure to be added to the macro table.
 */
void add_macro(MacroTable *table, Macro *new_macro);

/**
 * Initialize an InstructionLine structure with a given line of text.
 * @param line - The line of text to initialize the InstructionLine with.
 * @param file_name The filename of the src assembly file
 * @return A pointer to the initialized InstructionLine structure.
 */
InstructionLine *init_instruction_line(char* line, int file_number, char *file_name);

/**
 * Initialize a Command structure.
 * @return A pointer to the initialized Command structure.
 */
Command *init_command(void);

/**
 * Initialize an Operand structure.
 * @return A pointer to the initialized Operand structure.
 */
Operand *init_operand(void);

/**
 * Initialize a Directive structure.
 * @return A pointer to the initialized Directive structure.
 */
Directive *init_directive(void);

/**
 * Allocates memory for storing a binary instruction in the provided InstructionLine.
 *
 * This function calculates the required memory size based on the number of binary lines
 * and the length of each binary line. It then attempts to allocate that memory. If the
 * allocation fails, the program prints an error message and exits. If `binary_line_count`
 * is zero, the function prints an error and returns without allocating memory.
 *
 * @param p_line A pointer to an InstructionLine structure where the binary instruction
 *               memory will be allocated.
 * @param binary_line_count The number of binary lines to be allocated.
 * @param binary_line_length The length of each binary line.
 */
void allocate_binary_instruction(InstructionLine *p_line, size_t binary_line_count, size_t binary_line_length);

/**
 * Retrieves the binary instruction from a specific line in the LinesArray.
 *
 * This function returns a pointer to the binary instruction stored in the
 * specified line of the LinesArray. It's assumed that the line index provided
 * is within the valid range of the LinesArray.
 *
 * @param linesArray A pointer to the LinesArray structure that contains the lines.
 * @param number_of_line The index of the line from which to retrieve the binary instruction.
 * @return A pointer to the binary instruction stored in the specified line.
 */
char *get_instruction_line_binary(LinesArray *linesArray, int number_of_line);

/**
 * Checks if the given instruction line represents a directive.
 * This function determines if the `InstructionLine` contains a non-NULL `directive` field,
 * indicating that the instruction is a directive rather than an opcode.
 *
 * @param instructionLine The `InstructionLine` structure to be checked. This structure includes
 *                        the `directive` field that is used to determine if the instruction
 *                        is a directive.
 *
 * @return `1 if the `directive` field is non-NULL, indicating the instruction is a directive,
 *         otherwise 0.
 */
int is_instruction_line_directive(InstructionLine *instructionLine);

/**
 * Checks if the given instruction line represents an opcode.
 * This function determines if the `InstructionLine` contains a non-NULL `command` field,
 * indicating that the instruction is an opcode rather than a directive.
 *
 * @param instructionLine The `InstructionLine` structure to be checked. This structure includes
 *                        the `command` field that is used to determine if the instruction
 *                        is an opcode.
 *
 * @return 1 if the `command` field is non-NULL, indicating the instruction is an opcode,
 *         otherwise 0.
 */
int is_instruction_line_opcode(InstructionLine *instructionLine);

/**
 * Checks if the given operand classification type is valid.
 * This function verifies whether the `operandClassificationType` falls within the valid range
 * of `operand_classification_type` enumeration values. The valid range is from `IMMEDIATE` to
 * `DIRECT_REGISTER`.
 *
 * @param operandClassificationType The `operand_classification_type` value to be checked. This
 *                                   enumeration represents the classification of an operand.
 *
 * @return `1` (true) if the `operandClassificationType` is within the valid range, otherwise `0` (false).
 */
int is_operand_classification_type_valid(enum operand_classification_type operandClassificationType);

/**
 * Get symbol by given name
 * @param symbol_table A pointer to the SymbolTable structure that stores all symbols encountered in the source file.
 * @param symbol_name The name of the symbol to find
 * @return the symbol with the given name
 */
Symbol *find_symbol_by_name(SymbolTable* symbol_table, char* symbol_name);

/**
 * @brief Checks if the given operand classification type corresponds to a register.
 *
 * This function determines whether the provided operand classification type
 * is either an indirect register or a direct register. If the classification type
 * matches either of these, the function returns true; otherwise, it returns false.
 *
 * @param classification_type The operand classification type to be checked.
 * @return true if the classification type is INDIRECT_REGISTER or DIRECT_REGISTER;
 *         otherwise, false.
 */
int is_classification_type_register(enum operand_classification_type classification_type);

/**
 * Checks if a macro with the given name exists in the macro table.
 *
 * @param table The macro table to search in.
 * @param name The name of the macro to search for.
 * @return 1 if the macro name exists, 0 otherwise.
 */
Macro *macro_exists(const MacroTable *table, const char *name);

/*
 * Initialize a Macro structure with a given name.
 * @param macro_name The name of the new macro
 */
Macro *init_macro(char *macro_name);

#endif
