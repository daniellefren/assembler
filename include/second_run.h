#ifndef ASSEMBLER_SECOND_RUN_H
#define ASSEMBLER_SECOND_RUN_H
#include "../include/utils.h"

/**
 * Performs the second pass over the assembly lines stored in `assembly_lines_array`.
 * This function processes each line by allocating memory for binary instructions,
 * filling in the binary representation, and printing the instruction details.
 * After processing all lines, it creates an object file with the processed instructions.
 *
 * @param assembly_lines_array A pointer to the `LinesArray` structure that contains
 *                             the assembly lines to be processed. This structure holds
 *                             all the lines of code, along with associated metadata like
 *                             starting addresses and binary content.
 * @param file_number An integer representing the file number to be appended.
 */
void start_second_run(LinesArray *assembly_lines_array, int file_number);

/**
 * Fills the binary representation for an instruction line. Depending on whether the instruction
 * is an opcode or a directive, it creates the appropriate binary format. For opcodes, it generates
 * the binary for the first word, and additional words if needed, and appends them.
 * For directives, it generates the binary format specific to the directive type.
 *
 * @param instruction_line A pointer to the `InstructionLine` structure that contains the
 *                         instruction line to be converted into its binary form.
 */
void fill_instruction_line_binary(InstructionLine *instruction_line);

/**
 * Sets the binary string representation of the given opcode number.
 * This function converts the provided `opcode_number` into a binary string with a length defined by
 * `OPCODE_SIZE`. The result is stored in `binary_string`, which must be large enough to hold the binary
 * representation.
 *
 * @param opcode_number The integer value of the opcode to be converted into binary representation.
 *                      This value is masked to fit within the defined opcode size.
 * @param binary_string A pointer to a character array where the binary string representation of the
 *                      opcode will be stored. The array must be at least `OPCODE_SIZE` characters long.
 */
void set_binary_string_opcode_representation(int opcode, char* binary_string);

/**
 * Fills the first part of the binary representation for an opcode instruction line.
 * This function converts the opcode and operand classifications of the given `InstructionLine` into
 * their binary string representations. It updates the provided `binary_string` with the combined
 * binary data, including the opcode, ARE (Addressing, Register, or External), and operand classifications.
 *
 * @param instruction_line A pointer to the `InstructionLine` structure that contains the command
 *                         details. This includes the opcode, operand classifications, and operand
 *                         numbers.
 * @param binary_string A pointer to a character array where the binary representation will be stored.
 *                      This array should be large enough to hold the binary data.
 */
void fill_first_part_binary_opcode(InstructionLine *instruction_line, char* binary_string);

/**
 * Fills the second part of the binary representation for an opcode instruction line.
 * This function processes and adds the binary representation of operands to the `binary_string`
 * based on the number of operands present in the instruction. The length of the binary part depends on
 * the number of operands: 15 bits or 30 bits in total.
 *
 * @param instruction_line A pointer to the `InstructionLine` structure that contains the command details.
 *                         This includes information about the source and destination operands.
 * @param binary_string A pointer to a character array where the binary representation will be appended.
 *                      This array should be large enough to hold the additional binary data.
 */
void fill_second_part_binary_opcode(InstructionLine *instruction_line, char* binary_string);

/**
 * Fills a binary string with '0' characters and null-terminates it.
 * This function initializes the given `binary_string` with '0' characters up to the specified length.
 * It also ensures that the string is properly null-terminated. If the length is invalid (<= 0),
 * an error message is printed, and the program exits.
 *
 * @param binary_string A pointer to the character array that will be filled with '0' characters.
 *                      This array should be large enough to accommodate the specified length plus the null terminator.
 * @param length The number of '0' characters to write into the `binary_string`. This should be a positive value.
 */
void fill_the_binary_with_zero(char *binary_string, size_t length);

/**
 * Sets the binary string representation of the given opcode number.
 * Converts the `opcode_number` into a binary string of length defined by `OPCODE_SIZE` and stores it
 * in the `binary_string` array. The result is a string of '0's and '1's representing the binary value
 * of the opcode.
 *
 * @param opcode_number The integer value of the opcode to be converted into a binary string.
 *                      The value is masked to fit within the defined opcode size.
 * @param binary_string A pointer to a character array where the binary representation of the opcode
 *                      will be stored. The array should be at least `OPCODE_SIZE` characters long to
 *                      accommodate the binary data plus a null terminator.
 */
void set_binary_string_operand_representation(int first_operand_classification_type, int second_operand_classification_type, char *binary_string);

/**
 * Fills the binary representation of an operand into a binary string.
 * This function converts the operand values into their binary string representation
 * based on the classification type of the operand. It handles different types of
 * operands such as immediate values, direct addresses, and registers.
 *
 * @param operand The operand to be converted to binary. This includes information on classification type
 *                and its value or symbol.
 * @param second_operand The second operand if available (for cases where there are two operands).
 * @param binary_string The string where the binary representation will be stored.
 * @param operand_number Indicates whether the operand is the source (1) or destination (2) operand.
 */
void fill_operand_binary(Operand *operand, Operand *second_operand, char *binary_string, int operand_number);

/**
 * Sets the ARE (Absolute/Relative/External) representation bit in a binary string.
 *
 * Updates a specific position in the `binary_string` based on the given `ARE` character.
 * The position is calculated based on the binary string number and the ARE character
 *
 * @param binary_string The binary string to update.
 * @param binary_string_number The index of the binary string (1-based).
 * @param ARE The ARE character indicating the type ('a', 'r', or 'e').
 *
 * @note The function calculates the offset in the binary string where the ARE bit should be set.
 *       It exits with an error if the ARE character is not 'a', 'r', or 'e'.
 */
void set_binary_string_ARE_representation(char *binary_string, int number_of_binary_string, char ARE);

/**
 * Fills a binary string representation based on the directive type in an InstructionLine.
 *
 * Updates `binary_string` based on the type of directive (DATA or STRING) associated with
 * the `instruction_line`. For DATA directives, it converts integer values to binary. For
 * STRING directives, it converts each character to binary.
 *
 * @param instruction_line The InstructionLine structure containing the directive.
 * @param binary_string The buffer to store the resulting binary string.
 *
 */
void fill_binary_directive(InstructionLine *instruction_line, char *binary_string);

/**
 * Converts a register value to its binary representation and stores it in a binary string.
 *
 * Updates a portion of the `binary_string` based on the register value and the operand number.
 * The register number is converted to binary and placed in the appropriate bit positions:
 * - For `operand_number` 1 (source), it is placed in bits 6-8.
 * - For `operand_number` 2 (destination), it is placed in bits 9-11.
 *
 * @param register_value The string representation of the register number.
 * @param operand_number Indicates whether the register is a source (1) or destination (2).
 * @param binary_string The buffer to store the binary representation.
 * @param offset The starting position in `binary_string` for storing the result.
 *
 */
void register_to_binary_string(char *register_value, int operand_number, char *binary_string, int offset);
#endif //ASSEMBLER_SECOND_RUN_H
