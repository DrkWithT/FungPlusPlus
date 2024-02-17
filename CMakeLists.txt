# Top CMakeLists
# FungPlusPlus

# Use latest CMake version as of Nov. 11 2023
cmake_minimum_required(VERSION 3.27.7)

# Set project attributes
project(FungPlusPlus)
project(FungPlusPlus VERSION 0.1.0)

# Set build options
option(USE_DEBUG_BUILD "Option of whether to compile with debug info." ON)

# Compiler & Linker setup:
# Like: gcc --std=c11 -Wall -Werror ...
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (USE_DEBUG_BUILD)
    add_compile_options(-Wall -Werror -g)
else()
    add_compile_options(-Wall -Werror -O2)
endif()

# use ./include for headers, ./src recursively for implementation source...
include_directories("./include")
add_subdirectory("src")
