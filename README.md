# Markdown Parser Project

This project is a C-based markdown parser that reads a markdown file, tokenizes its content, and generates an HTML output based on the extracted tokens. It supports various markdown elements like headings, bold, and italics.

## Features
- Tokenizes markdown content into different token types (e.g., plain text, headings, bold, italics).
- Generates an HTML file from the tokenized content.
- Handles edge cases such as consecutive special characters and mixed content types.

## Project Structure
- **CMakeLists.txt**: CMake build configuration file.
- **parser.c**: Contains functions for tokenizing the markdown content, managing tokens, and converting the tokens to HTML.
- **parser.h**: Header file for defining structures and function prototypes.
- **test.md**: Sample markdown file for testing the parser.
- **output.html**: The generated HTML output file.

## Dependencies
- None. The project is built using standard C libraries.

## Setup

### Prerequisites
Make sure you have CMake and a C compiler (like `gcc`) installed on your machine.

- CMake: [Install CMake](https://cmake.org/install/)
- GCC (or any C compiler): [Install GCC](https://gcc.gnu.org/)

### Building and Running the Project
1. Clone the repository or download the project files to your local machine.
2. Open a terminal and navigate to the project directory.
3. Run the following command to generate the build files:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ./HyperMark
   ```
