/**
 * @file lexer.cpp
 * @author DrkWithT
 * @brief Implements tokenizer.
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "frontend/lexer.hpp"

namespace fung::frontend
{
    /* Tokenizer constants and helpers */
    static constexpr char test_sp = ' ';
    static constexpr char test_tb = '\t';
    static constexpr char test_cr = '\r';
    static constexpr char test_lf = '\n';
    static const char* test_operator_symbols = "@?+-*/=!<>&|";
    
    static constexpr size_t test_keyword_count = 15;
    static constexpr size_t test_operator_count = 15;
    static constexpr size_t test_operator_symbols_len = 12;

    static const char* test_keywords[] {
        "use",
        "let",
        "mut",
        "val",
        "ref",
        "fun",
        "object",
        "field",
        "end",
        "ret",
        "if",
        "else",
        "while",
        "each",
        "in"
    };

    static const char* test_operators[] {
        "?",
        "+",
        "-",
        "*",
        "/",
        "==",
        "!=",
        "<",
        ">",
        "<=",
        ">=",
        "&&",
        "||",
        "=",
        "@",
    };

    static TokenType test_operator_lextypes[] {
        token_op_nonil,
        token_op_plus,
        token_op_minus,
        token_op_times,
        token_op_slash,
        token_op_isequal,
        token_op_unequal,
        token_op_lt,
        token_op_gt,
        token_op_lte,
        token_op_gte,
        token_op_logic_and,
        token_op_logic_or,
        token_op_assign,
        token_op_extract
    };

    constexpr bool isWhitespace(char c)
    {
        return c == test_sp || c == test_tb || c == test_cr || c == test_lf;
    }

    constexpr bool isAlphabetic(char c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
    }

    bool isOperatorSymbol(char c)
    {
        for (size_t symbol_i = 0; symbol_i < test_operator_symbols_len; symbol_i++)
        {
            if (c == test_operator_symbols[symbol_i])
            {
                return true;
            }
        }

        return false;
    }

    constexpr bool isNumeric(char c)
    {
        return (c >= '0' && c <= '9') || c == '.';
    }

    /* Lexer */

    Lexer::Lexer(const char* source_cstr, size_t source_size)
    : operators {}, keywords {}, source_view {source_cstr}, position {0}, limit {source_size}
    {
        for (size_t operator_i = 0; operator_i < test_operator_count; operator_i++)
        {
            operators.insert({test_operators[operator_i], test_operator_lextypes[operator_i]});
        }

        for (size_t keyword_i = 0; keyword_i < test_keyword_count; keyword_i++)
        {
            keywords.insert(test_keywords[keyword_i]);
        }
    }

    void Lexer::reset(const char* source_cptr, size_t source_size)
    {
        source_view = source_cptr;
        position = 0;
        limit = source_size;
    }

    Token Lexer::lexSingleSymbol(TokenType lexical_type)
    {
        size_t token_start = position;

        position++;

        return {.begin = token_start, .length = 1, .type = lexical_type};
    }

    Token Lexer::lexBetween(char c, TokenType lexical_type)
    {
        /// @note skip pre-checked character matching c to avoid stuck loop
        position++;

        size_t token_begin = position;
        size_t token_length = 0;
        char temp;

        while (position < limit)
        {
            temp = source_view[position];

            if (temp == c)
            {
                position++;
                break;
            }

            token_length++;
            position++;
        }

        return {.begin = token_begin, .length = token_length, .type = lexical_type};
    }

    Token Lexer::lexWhitespace()
    {
        size_t token_begin = position;
        size_t token_length = 0;
        char temp;

        while (position < limit)
        {
            temp = source_view[position];

            if (!isWhitespace(temp))
            {
                break;
            }

            token_length++;
            position++;
        }

        return {.begin = token_begin, .length = token_length, .type = token_whitespace};
    }

    Token Lexer::lexWord()
    {
        size_t token_begin = position;
        size_t token_length = 0;
        char temp;

        while (position < limit)
        {
            temp = source_view[position];

            if (!isAlphabetic(temp))
            {
                break;
            }

            token_length++;
            position++;
        }

        std::string lexeme_str {};

        for (size_t lexeme_i = 0; lexeme_i < token_length; lexeme_i++)
        {
            lexeme_str += source_view[token_begin + lexeme_i];
        }

        if (keywords.find(lexeme_str) != keywords.end())
        {
            return {.begin = token_begin, .length = token_length, .type = token_keyword};
        }

        return {.begin = token_begin, .length = token_length, .type = token_identifier};
    }

    Token Lexer::lexOperator()
    {
        size_t token_begin = position;
        size_t token_length = 0;
        char temp;

        while (position < limit)
        {
            temp = source_view[position];

            if (!isOperatorSymbol(temp))
            {
                break;
            }

            token_length++;
            position++;
        }

        std::string lexeme_str {};

        for (size_t lexeme_i = 0; lexeme_i < token_length; lexeme_i++)
        {
            lexeme_str += source_view[token_begin + lexeme_i];
        }

        if (operators.find(lexeme_str) != operators.end())
        {
            return {.begin = token_begin, .length = token_length, .type = operators.at(lexeme_str)};
        }

        return {.begin = token_begin, .length = token_length, .type = token_bad};
    }

    Token Lexer::lexSpecialLiteral()
    {
        /// @note Skip leading '$' to avoid infinite looping in tokenizer...
        position++;

        size_t token_begin = position - 1;
        char special_c = source_view[position];

        position++;

        /// @note Check for True, False, Nil special literals of form $<letter>...
        if (special_c == 'T')
        {
            return {.begin = token_begin, .length = 2, .type = token_special_true};
        }
        else if (special_c == 'F')
        {
            return {.begin = token_begin, .length = 2, .type = token_special_false};
        }
        else if (special_c == 'N')
        {
            return {.begin = token_begin, .length = 2, .type = token_special_nil};
        }
        else
        {
            return {.begin = token_begin, .length = 2, .type = token_bad};
        }
    }

    Token Lexer::lexNumeric()
    {
        size_t token_begin = position;
        size_t token_length = 0;
        size_t dot_count = 0;
        char temp;

        while (position < limit)
        {
            temp = source_view[position];

            if (!isNumeric(temp))
            {
                break;
            }

            if (temp == '.')
            {
                dot_count++;
            }

            token_length++;
            position++;
        }
        
        switch (dot_count)
        {
        case 0:
            return {.begin = token_begin, .length = token_length, .type = token_integer};
        case 1:
            return {.begin = token_begin, .length = token_length, .type = token_float};
        default:
            return {.begin = token_begin, .length = token_length, .type = token_bad};
        }
    }

    Token Lexer::lexNext()
    {
        if (position >= limit)
        {
            return {.begin = limit, .length = 1, .type = token_eof};
        }

        char letter = source_view[position];

        if (isWhitespace(letter))
        {
            return lexWhitespace();
        }

        if (letter == '#')
        {
            return lexBetween('#', token_comment);
        }

        switch (letter)
        {
        case '\"':
            return lexBetween('\"', token_string);
        case '$':
            return lexSpecialLiteral();
        case ',':
            return lexSingleSymbol(token_comma);
        case '[':
            return lexSingleSymbol(token_lbrack);
        case ']':
            return lexSingleSymbol(token_rbrack);
        case '{':
            return lexSingleSymbol(token_lbrace);
        case '}':
            return lexSingleSymbol(token_rbrace);
        case '(':
            return lexSingleSymbol(token_lparen);
        case ')':
            return lexSingleSymbol(token_rparen);
        default:
            break;
        }

        if (isAlphabetic(letter))
        {
            return lexWord();
        }
        else if (isNumeric(letter))
        {
            return lexNumeric();
        }
        else if (isOperatorSymbol(letter))
        {
            return lexOperator();
        }

        position++;

        return {.begin = position - 1, .length = 1, .type = token_bad};
    }
}
