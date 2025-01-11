
# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MarkdownToHTML VERSION 1.0)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include the header files directory
include_directories(include)

# Add the executable target
add_executable(MarkdownToHTML
    src/main.c
    src/parser.c
)
