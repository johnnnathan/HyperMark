# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and language
project(MyCProject LANGUAGES C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the source directory
set(SOURCE_FILES
    src/main.c
    src/parser.c
)

# Create the executable
add_executable(HyperMark ${SOURCE_FILES})

# Link libraries (if any)
# target_link_libraries(MyCProject some_library)
