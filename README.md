# Minishell
# 👀 Project Overview

Minishell is a project inspired by bash, an historical shell, allowing users to interact with a computer using a command-line interface. This project emulates the basic functionalities of a shell, offering a deep dive into system programming and Unix fundamentals.

# 💻 Get Started: Compilation and Execution
To compile and run Minishell, use the following commands:

Clone this repository:

```
make
./minishell
```

# 🛠 Technical Specifications

- Language: C
- External Libraries: `readline` for input handling
- Concepts: System programming, Unix processes, file descriptors

# 🌟 Key Features
- **_Command Line Interface_**: Interactive shell with a prompt for entering commands.
- **_Command Parsing_**: Robust parsing mechanism to handle complex command inputs, including pipes, redirections, and special characters.
- **_Environment Variables_**: Supports expansion of environment variables.
- **_Built-in Commands_**: Includes built-ins like `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **_Signal Handling_**: Manages signals like `ctrl-C`, `ctrl-D`, and `ctrl-\`.
- **_Error Handling_**: Comprehensive error management to gracefully handle unexpected situations during command execution.
- **_Redirection and Pipes_**: Implements input/output redirection and pipe functionalities.

# 🧮 Command Parsing Details
The shell includes an advanced lexer and parser to process commands:

- **_Lexer_**: Handles syntax checking for pipes, quotes, and redirections.
- **_Command Subdivision_**: Splits input into commands based on pipes, handling quotes accurately.
- **_Parsing_**: Expands environment variables, processes quoted strings, and prepares commands for execution.
- **_Tokenization_**: Classifies each segment of the command for accurate execution, identifying commands, arguments, operators, and file paths.

# ⚙️ Execution Mechanism
- Executes parsed commands, handling built-in functions.
- Manages file descriptors for redirections and pipes.
- Ensures proper cleanup and memory management post-command execution.

# 💡 Key learnings :

- Deep understanding of Unix processes and system calls.
- Advanced command-line parsing techniques.
- Effective management of file descriptors and inter-process communication.
- Proficient use of C language in a complex application.

# 👥 Contribution and Development
This project is a collaborative effort :

<p align="center">
<a href="http://github.com/lmelard" alt="lmelard github profile"><img src="https://github.com/lmelard.png" width="60px style="border-radius:50%"/></a>
<a href="http://github.com/tiny-chris" alt="tiny-chris github profile"><img src="https://github.com/tiny-chris.png" width="60px style="border-radius:50%"/></a>
</p>
