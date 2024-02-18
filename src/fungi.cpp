/**
 * @file fungi.cpp
 * @author DrkWithT
 * @brief Implements main cmd file for using the Fung language.
 * @version 0.1.0
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include "frontend/lexer.hpp"

using my_token_type = fung::frontend::TokenType;

[[nodiscard]] bool readAFile(const char* path, std::unique_ptr<char[]>& result, size_t* external_size)
{
    std::ifstream reader {path, std::ios::in};

    if (!reader.is_open())
    {
        return false;
    }

    size_t input_size = 0;
    reader.seekg(0, std::ios::end);
    input_size = reader.tellg();
    reader.seekg(0, std::ios::beg);

    auto buffer = std::make_unique<char[]>(input_size + 1);

    std::fill(buffer.get(), buffer.get() + input_size, '\0');

    bool read_ok = !reader.read(buffer.get(), input_size).bad();

    if (read_ok)
    {
        result.reset(buffer.release());
        *external_size = input_size;
    }

    return read_ok;
}

int main (int argc, char* argv[]) {
    std::unique_ptr<char[]> source_buffer {};
    size_t my_file_size = 0;

    if (!readAFile("./examples/test07.fung", source_buffer, &my_file_size))
    {
        std::cerr << "Failed to read file :(\n";
        return 1;
    }

    fung::frontend::Token temp_token {};
    fung::frontend::Lexer lexer {source_buffer.get(), my_file_size};

    do
    {
        temp_token = lexer.lexNext();

        std::cout << "Token {type=" << temp_token.type << ", begin=" << temp_token.begin << ", length=" << temp_token.length << "}\n";
    } while (temp_token.type != my_token_type::token_eof);
}
