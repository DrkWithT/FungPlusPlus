#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <set>
#include <map>
#include "frontend/token.hpp"

namespace fung::frontend
{
    class Lexer
    {
    private:
        std::map<std::string, TokenType> operators;
        std::set<std::string> keywords;
        std::string_view source_view;
        size_t position;
        size_t limit;
    public:
        Lexer(const char* source_cstr, size_t source_size);

        void reset(const char* source_cptr, size_t source_size);

        Token lexSingleSymbol(TokenType lexical_type);

        Token lexBetween(char c, TokenType lexical_type);

        Token lexWhitespace();

        Token lexWord();

        Token lexOperator();

        Token lexSpecialLiteral();

        Token lexNumeric();

        Token lexNext();
    };
}

#endif