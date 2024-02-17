/**
 * @file token.cpp
 * @author DrkWithT
 * @brief Implements token types.
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <exception>
#include "frontend/token.hpp"

namespace fung::frontend::tokens
{
    [[nodiscard]] bool testTokenPrintable(const Token &token)
    {
        return token.type != token_eof && token.length > 0;
    }

    std::string_view stringifyToken(const Token &token, const std::string_view &source)
    {
        if (!testTokenPrintable(token))
        {
            throw std::invalid_argument{"Cannot print EOF or empty Tokens!"};
        }

        return source.substr(token.begin, token.length);
    }
}
