# Assembler Project

This project is an assembler implementation designed to convert assembly language code into machine code. 


## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#Installation)
- [Usage](#usage)

## Overview

This assembler handles various assembly directives and commands. It also supports macro expansion, symbol management, and generating the necessary binary instructions. The main goal is to process assembly code into a format ready for execution.

## Features
- **Macro Expansion**: The assembler expands macros found in the source files.
- **Symbol Handling**: The assembler manages symbols for both commands and data directives.
- **Error Handling**: The assembler is designed to handle errors gracefully
- **Multiple Source Files**: Supports processing multiple source files by generating separate expanded macro files.
- **Directives Support**: Handles `.data`, `.string`, `.extern`, and `.entry` directives.
- **Command Support**: Implements various assembly commands with proper operand handling.

## Project Structure

The assembler works in three main stages:
1. Pre-Assembler
2. First Pass
3. Second Pass

### Pre-Assembler
The pre-assembler scans through the assembly source file and looks for macro definitions. When it encounters a macro, it stores the macro's name and body in `macroTable`. When the macro is invoked later in the source code, the function expands the macro by replacing the invocation with its body in a new output file. At the end of the pre-processing, each assembly input file has its own expanded macros file.

### First Pass
The first pass of the assembler scans through the expanded macros file that the pre-assembler has created. During the scan, it identifies symbols and assigns them numerical values representing their corresponding memory addresses, storing them in `labelTable`. Additionally, it creates an `InstructionLine` for each line from the assembly code. Each instruction line is stored in the `linesArray` and contains data related to the current command/directive of the assembly line, as well as a number representing the number of binary lines for the current assembly line. Furthermore, two files—`ps.end` and `ps.ext`—are generated in this pass, containing all the entries and externals in the file.

### Second Pass
In the second pass, the assembler processes each line of the assembly code to generate its binary representation. The main steps include:

- **Processing Each Line**: The function `start_second_run` iterates over each line in the assembly code, converting it into binary format and handling any associated symbols or directives.
- **Binary Conversion**: Functions such as `fill_instruction_line_binary`, `fill_first_part_binary_opcode`, and `fill_second_part_binary_opcode` manage the conversion of opcodes, operands (immediate values, direct addresses, registers), and directives into binary strings.
- **Binary String Formatting**: The code ensures that binary strings are formatted correctly by setting specific bits for opcodes, operand classifications, and the ARE (Absolute, Relocatable, External) values using functions like `set_binary_string_opcode_representation` and `set_binary_string_ARE_representation`.
- **Final Output**: After processing all lines, the assembler generates the final binary output and writes it to an object file.

This pass is crucial for creating the final machine code that can be executed by the target system.


## Getting started

### Installation
1. Clone the Labratory-C-Final-Project repository:
```sh
git clone https://github.com/daniellefren/assembler.git
```
2. Change to the project directory:
```sh
cd assembler
```
3. Run Makefile to compile the program:
```sh
`make`
```
### Usage
In order to run the compiled program you should run the following command `./assembler file_name_1 ... file_name_n`
This will create all the output files in the `output_files\` directory.
For each file from the input files, will be created (for the i file):
1. `expanded_macros{i}.am` (The src file after the pre-assembler)
2. `ps{i}.ent` (All the entries)
3. `ps{i}.ext` (All the externals)
4. `ps{i}.ob` (Final binary file)

### Testing
Several input assembly files are provided, with different command and directives in order to present the assembler work properly.



# Royi David and Danielle Frenklakh ©



