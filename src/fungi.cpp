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

#include <iostream>
#include "frontend/token.hpp"

using namespace fung::frontend::tokens;

static const char* fake_source = "use stdio\n";

int main (int argc, char* argv[]) {
    std::string_view source_view {fake_source};
    Token keyword1 {.begin = 0, .length = 3, .type = token_keyword};

    std::string_view use_keyword = stringifyToken(keyword1, source_view);

    std::cout << use_keyword << '\n'; // should print use
}
