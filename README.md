# Assembler Project

This project is an assembler implementation designed to convert assembly language code into machine code. 


## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [How to Build](#how-to-build)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

This assembler handles various assembly directives and commands. It also supports macro expansion, label management, and generating the necessary binary instructions. The main goal is to process assembly code into a format ready for execution.

## Features
- **Macro Expansion**: The assembler expands macros found in the source files.
- **Label Handling**: The assembler manages labels for both commands and data directives.
- **Error Handling**: The assembler is designed to handle errors gracefully, but TODO: Ensure that all errors are captured, including line numbers, without exiting prematurely.
- **Multiple Source Files**: Supports processing multiple source files by generating separate expanded macro files.
- **Directives Support**: Handles `.data`, `.string`, `.extern`, and `.entry` directives.
- **Command Support**: Implements various assembly commands with proper operand handling.

## Project Structure

The assembler works in three main stages:
1. Pre-Assembler
2. First Pass
3. Second Pass

### Pre-Assembler
The pre-assembler, scans through the assembly source file and looks for macro definitions. When encountered a macro, it stores the macro's name and body in macroTable.
When the macro is invoked later in the source code, the function expands the macro by replacing the invocation with its body, in a new output file.
In the end of the pre-processing, we have a new file (each assembly input file, has its own expanded macros file) with the expanded macros.


### First Pass
The first pass of the assembler, scans through the expanded macros file that the pre-assembler has created.
Through the scan' it identifies labels (Symbols) and assigns them numerical values representing their corresponding memory addresses, and stores into a lableTable.
In addition, it created an InstructionLine for each line from the assembly code.
Each instruction line is stored in the linesArray, and consists with tha data of the current command/direct of the assembly line, and a number that represents the number of binary lines for the current assembly line. 


### Second Pass


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



