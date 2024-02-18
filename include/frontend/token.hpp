/**
 * @file token.hpp
 * @author DrkWithT
 * @brief Declare token types.
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <string_view>

namespace fung::frontend
{
    enum TokenType
    {
        token_eof,
        token_bad,
        token_whitespace,
        token_comment,
        token_keyword,
        token_identifier,
        token_special_true,
        token_special_false,
        token_special_nil,
        token_integer,
        token_float,
        token_string,
        token_op_plus,
        token_op_minus,
        token_op_nonil,
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
        token_comma,
        token_lparen,
        token_rparen,
        token_lbrace,
        token_rbrace,
        token_lbrack,
        token_rbrack
    };

    struct Token
    {
        size_t begin;
        size_t length;
        TokenType type;
    };

    [[nodiscard]] bool testTokenPrintable(const Token& token);

    std::string_view stringifyToken(const Token& token, const std::string_view& source);
}
