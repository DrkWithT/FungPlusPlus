/**
 * @file fungi.cpp
 * @author DrkWithT
 * @brief Implements main utility file for the Fung bytecode interpreter.
 * @version 0.1.1
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include "frontend/parser.hpp"

using my_token_type = fung::frontend::TokenType;

[[nodiscard]] std::unique_ptr<char[]> readAFile(const char* path, size_t* external_size)
{
    std::ifstream reader {path, std::ios::in};

    if (!reader.is_open())
    {
        return nullptr;
    }

    size_t input_size = 0;
    reader.seekg(0, std::ios::end);
    input_size = reader.tellg();
    reader.seekg(0, std::ios::beg);

    auto buffer = std::make_unique<char[]>(input_size + 1);

    std::fill(buffer.get(), buffer.get() + input_size + 1, '\0');

    bool read_ok = !reader.read(buffer.get(), input_size).bad();

    if (read_ok)
    {
        *external_size = input_size;
    }

    return buffer;
}

int main (int argc, char* argv[]) {
    if (argc < 2)
    {
        std::cerr << "Usage: ./bin/fungi <file>\n";
        return 1;
    }

    size_t my_file_size = 0;
    std::string my_filename_str {argv[1]};
    auto source_buffer = readAFile(argv[1], &my_file_size);

    if (!source_buffer || my_file_size == 0)
    {
        std::cerr << "Failed to read given file at path.\n";
        return 1;
    }

    fung::frontend::Parser fung_parser {source_buffer.get(), my_file_size};
    fung::frontend::ProgramUnit program {my_filename_str};

    if (!fung_parser.parseFile(program))
    {
        std::cerr << "Parsing failed for file " << program.getName() << '\n';
        return 1;
    }
}
