/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements recursive descent parser for Fung.
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <utility>
#include "frontend/parser.hpp"

namespace fung::frontend
{
    /* ProgramUnit impl. */

    ProgramUnit::ProgramUnit(const std::string& unit_name)
    : statements {}, name(std::move(unit_name)) {}

    const std::vector<std::unique_ptr<fung::syntax::IStmt>>& ProgramUnit::getStatements() const
    {
        return statements;
    }

    const std::string& ProgramUnit::getName() const
    {
        return name;
    }

    /* Parser helpers impl. */

    const Token& Parser::getPrevious() const
    {
        return previous;
    }

    const Token& Parser::getCurrent() const
    {
        return current;
    }

    [[nodiscard]] bool Parser::matchToken(TokenType type)
    {
        return getCurrent().type == type;
    }

    [[nodiscard]] Token Parser::advanceToken()
    {
        Token temp;

        do
        {
            temp = lexer.lexNext();

            if (temp.type == TokenType::token_eof)
            {
                break;
            }

            if (temp.type == TokenType::token_bad)
            {
                std::cerr << "Unknown token at pos. " << temp.begin << '\n';
                continue;
            }

            if (temp.type != TokenType::token_whitespace)
            {
                break;
            }
        } while (true);

        return temp;
    }

    void Parser::consumeToken(const std::array<TokenType, 4>& types)
    {
        if (getCurrent().type == token_eof)
        {
            previous = current;
            current = advanceToken();
            return;
        }

        for (const auto type : types)
        {
            if (getCurrent().type == type)
            {
                previous = current;
                current = advanceToken();
            }
        }

        reportError(getCurrent(), "Unexpected token: ");
    }

    void Parser::reportError(const Token& token, const std::string& msg_header)
    {
        std::ostringstream sout {};

        sout << msg_header << token.type << " at position " << token.begin << '\n';

        throw std::invalid_argument {sout.str()};
    }

    void synchronizeParse(); // TODO!

    /* Parser parsing procedures impl. */

    /* Parser public impl. */
}
