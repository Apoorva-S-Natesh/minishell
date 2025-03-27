# Minishell

Minishell is a minimalist command-line interpreter, designed to simulate the behavior of a basic shell program. This project was developed using the C programming language and focuses on implementing core shell features, such as executing commands, handling redirections, managing environment variables, and supporting built-in shell commands.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Technologies](#technologies)
- [Contributors](#contributors)

## Description

Minishell is a shell implementation that aims to replicate many features of Bash in a simplified way. The project implements the following functionalities:

- A command-line prompt that waits for user input.
- Command execution, with support for search through the PATH variable and executing relative or absolute paths.
- History management of previously executed commands.
- Redirection operators (`<`, `>`, `<<`, `>>`) for input and output redirection.
- Pipes (`|`) for connecting commands in a pipeline.
- Environment variable expansion (e.g., `$HOME`, `$USER`, etc.).
- Built-in commands such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- Signal handling for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` in interactive mode.

## Features

### Built-in Commands

- **echo** with the `-n` option to suppress trailing newline.
- **cd** with relative or absolute paths.
- **pwd** to print the current working directory.
- **export** with no options to set environment variables.
- **unset** to remove environment variables.
- **env** to print the current environment variables.
- **exit** to terminate the shell.

### Input Handling

- **Quotes**: Handles single (`'`) and double (`"`) quotes to prevent the interpretation of special characters within the quoted sequence.
- **Redirections**:
  - `<` for input redirection.
  - `>` for output redirection.
  - `<<` for here-documents (delimited input redirection).
  - `>>` for appending output.
- **Pipes**: Allows the output of one command to be passed as the input to another command.

### Signal Handling

- **Ctrl-C**: Displays a new prompt on a new line.
- **Ctrl-D**: Exits the shell.
- **Ctrl-\ **: No action is performed.

## Installation

To run this project, clone the repository to your local machine and compile it with the provided Makefile.

### Clone the repository:

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
./minishell
```

## Technologies

This project was developed using the following technologies and tools:

```
- **C Programming Language**:
  - The entire shell is implemented using the C programming language. The project emphasizes low-level programming skills, such as memory management, string manipulation, process management, and system calls.

- **Libft**:
  - A custom library created for this project, providing essential functions for string manipulation, memory allocation, and basic input/output operations. It was developed as part of the curriculum and included in the project to avoid relying on external libraries.

- **Makefile**:
  - The Makefile is used for automating the compilation process. It ensures that all source files are compiled and linked correctly, and manages dependencies between files. It includes the necessary compilation flags (`-Wall`, `-Wextra`, `-Werror`) to enforce proper code quality and error handling.

- **External Libraries**:
  - **readline**: Used for providing an interactive command-line interface with features such as history, autocompletion, and line editing.
  - **system calls and standard libraries**: The project utilizes several system calls such as `fork`, `execve`, `wait`, `pipe`, and others to handle process creation, execution, and inter-process communication.
  - **Signal Handling**: The project uses the `signal` library to manage user input signals like `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` to simulate behavior similar to that of a real shell.

- **Version Control**:
  - **Git**: The project is tracked using Git for version control. It allows for easy collaboration, history tracking, and code management.
```

## Contributors

    # Apoorva-S-Natesh
    # arekiu
