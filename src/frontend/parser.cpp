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
    using FungLiteralType = fung::syntax::FungSimpleType;
    using FungOpType = fung::syntax::FungOperatorType;

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

    void Parser::consumeToken(std::initializer_list<TokenType> types)
    {
        /// @note Sometimes I must handle EOF or any token type... "consume past" anyways in cases: unary-expr, EOF.
        if (getCurrent().type == token_eof || types.size() == 0)
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

        sout << msg_header << "Token {begin = " << token.begin << ", type = " << token.type << "}\n";

        throw std::invalid_argument {sout.str()};
    }

    void Parser::synchronizeParse()
    {
        Token temp;

        do
        {
            temp = lexer.lexNext();

            current = temp;
            previous = current;

            if (matchToken(TokenType::token_eof) || matchToken(TokenType::token_keyword))
            {
                break;
            }

            if (matchToken(TokenType::token_whitespace) || matchToken(TokenType::token_bad))
            {
                continue;
            }
        } while (true);
    }

    /* Parser expression helpers impl. */

    std::vector<std::unique_ptr<fung::syntax::IExpr>> Parser::parseListLiteral()
    {
        consumeToken({TokenType::token_lbrack});

        std::vector<std::unique_ptr<fung::syntax::IExpr>> literal_args {};

        while (!matchToken(TokenType::token_rbrack))
        {
            auto temp_arg = parseElement();
            literal_args.emplace_back(temp_arg);

            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rbrack});

        return literal_args;
    }

    std::vector<std::unique_ptr<fung::syntax::IExpr>> Parser::parseObjectLiteral()
    {
        consumeToken({TokenType::token_lbrace});

        std::vector<std::unique_ptr<fung::syntax::IExpr>> object_args {};

        while (!matchToken(TokenType::token_rbrace))
        {
            auto temp_arg = parseElement();
            object_args.emplace_back(temp_arg);

            consumeToken({TokenType::token_comma});
        }

        return object_args;
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseElement()
    {
        TokenType temp_type = getCurrent().type;

        if (temp_type == token_special_nil)
        {
            auto value_nil = std::make_unique<fung::syntax::ElementExpr>(nullptr, FungLiteralType::fung_simple_type_nil);

            consumeToken({TokenType::token_special_nil});
            return value_nil;
        }
        else if (temp_type == token_special_true || temp_type == token_special_false)
        {
            auto value_bool = std::make_unique<fung::syntax::ElementExpr>((temp_type == token_special_true), FungLiteralType::fung_simple_type_bool);

            consumeToken({TokenType::token_special_true, TokenType::token_special_false});
            return value_bool;
        }
        else if (token_integer)
        {
            int x = std::stoi(stringifyTokenFully(getCurrent(), source_viewer));

            auto value_int = std::make_unique<fung::syntax::ElementExpr>(x, FungLiteralType::fung_simple_type_int);

            consumeToken({TokenType::token_integer});
            return value_int;
        }
        else if (token_float)
        {
            double x = std::stod(stringifyTokenFully(getCurrent(), source_viewer));

            auto value_float = std::make_unique<fung::syntax::ElementExpr>(x, FungLiteralType::fung_simple_type_float);

            consumeToken({TokenType::token_float});
            return value_float;
        }
        else if (token_string)
        {
            auto value_str = std::make_unique<fung::syntax::ElementExpr>(stringifyTokenFully(getCurrent(), source_viewer));

            consumeToken({TokenType::token_string});
            return value_str;
        }
        else if (token_lbrack)
        {
            auto list_values = parseListLiteral();
            auto list_literal = std::make_unique<fung::syntax::ElementExpr>(list_values, FungLiteralType::fung_simple_type_list);

            return list_literal;
        }
        else if (token_lbrace)
        {
            auto obj_values = parseObjectLiteral();
            auto obj_literal = std::make_unique<fung::syntax::ElementExpr>(obj_values, FungLiteralType::fung_simple_type_object);

            return obj_literal;
        }
        else if (token_lparen)
        {
            consumeToken({TokenType::token_lparen});
            auto top_expr = parseConditional();
            consumeToken({TokenType::token_rparen});

            return top_expr;
        }

        reportError(getCurrent(), "Unexpected token");
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseCall()
    {
        consumeToken({TokenType::token_identifier});

        std::string function_name = stringifyTokenFully(getPrevious(), source_viewer);
        std::vector<std::unique_ptr<fung::syntax::IExpr>> function_args {};

        consumeToken({TokenType::token_lparen});

        while (!matchToken(TokenType::token_rparen))
        {
            auto temp_arg = parseElement();
            function_args.emplace_back(std::move(temp_arg));

            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rparen});

        auto fn_call = fung::syntax::CallExpr(function_name);

        for (auto&& arg_uptr : function_args)
        {
            fn_call.addArgument(std::move(arg_uptr));
        }

        return std::make_unique<fung::syntax::CallExpr>(std::move(fn_call));
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseAccess()
    {
        consumeToken({TokenType::token_identifier});

        std::string lvalue_name = stringifyTokenFully(getPrevious(), source_viewer);
        std::vector<std::unique_ptr<fung::syntax::IExpr>> keys {};

        consumeToken({TokenType::token_lbrack});

        while (!matchToken(TokenType::token_rbrack))
        {
            auto keying_expr = parseElement();
            keys.emplace_back(std::move(keying_expr));

            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rbrack});
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseUnary()
    {
        char op = '\0';

        // Handle any starting unary operator by grammar
        if (matchToken(TokenType::token_op_minus) || matchToken(TokenType::token_op_nonil))
        {
            op = stringifyToken(getCurrent(), source_viewer)[0];
            consumeToken({});
        }

        FungOpType op_type = (op == '-')
            ? FungOpType::fung_op_minus
            : FungOpType::fung_op_nonil;

        // Check for access-expr or element-expr by grammar
        if (!matchToken(TokenType::token_identifier))
        {
            auto inner_element = parseElement();

            return std::make_unique<fung::syntax::UnaryExpr>(inner_element, op_type);
        }

        auto inner_access = parseAccess();

        return std::make_unique<fung::syntax::UnaryExpr>(inner_access, op_type);
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseFactor()
    {
        auto lhs = parseUnary();

        while (matchToken(TokenType::token_op_times) || matchToken(TokenType::token_op_slash))
        {
            consumeToken({TokenType::token_op_times, TokenType::token_op_slash});

            FungOpType factor_op = (getPrevious().type == TokenType::token_op_times)
                ? FungOpType::fung_op_times
                : FungOpType::fung_op_slash;
            
            auto rhs = parseUnary();

            lhs = std::make_unique<fung::syntax::BinaryExpr>(std::move(lhs), std::move(rhs), factor_op);
        }

        return lhs;
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseTerm()
    {
        auto lhs = parseFactor();

        while (matchToken(TokenType::token_op_plus) || matchToken(TokenType::token_op_minus))
        {
            consumeToken({TokenType::token_op_plus, TokenType::token_op_minus});

            FungOpType term_op = (getPrevious().type == TokenType::token_op_plus)
                ? FungOpType::fung_op_plus
                : FungOpType::fung_op_minus;
            
            auto rhs = parseFactor();

            lhs = std::make_unique<fung::syntax::BinaryExpr>(std::move(lhs), std::move(rhs), term_op);
        }

        return lhs;
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseComparison()
    {
        auto lhs = parseTerm();
        FungOpType cmp_op;

        /// TODO: refactor this ugly asf conditional into a multi-type token matcher
        while (matchToken(TokenType::token_op_isequal) || matchToken(TokenType::token_op_unequal) || matchToken(TokenType::token_op_lte) || matchToken(TokenType::token_op_gte) || matchToken(TokenType::token_op_lt) || matchToken(TokenType::token_op_gt))
        {
            consumeToken({TokenType::token_op_isequal, TokenType::token_op_unequal, TokenType::token_op_gte, TokenType::token_op_lte, TokenType::token_op_gt,TokenType::token_op_lt});

            switch (getPrevious().type)
            {
            case TokenType::token_op_isequal:
                cmp_op = FungOpType::fung_op_isequal;
                break;
            case TokenType::token_op_unequal:
                cmp_op = FungOpType::fung_op_unequal;
                break;
            case TokenType::token_op_gte:
                cmp_op = FungOpType::fung_op_gte;
                break;
            case TokenType::token_op_lte:
                cmp_op = FungOpType::fung_op_lte;
                break;
            case TokenType::token_op_gt:
                cmp_op = FungOpType::fung_op_gt;
                break;
            case TokenType::token_op_lt:
                cmp_op = FungOpType::fung_op_lt;
                break;
            default:
                break;
            }

            auto rhs = parseTerm();

            lhs = std::make_unique<fung::syntax::BinaryExpr>(std::move(lhs), std::move(rhs), cmp_op);
        }

        return lhs;
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseConditional()
    {
        auto lhs = parseComparison();

        while (matchToken(TokenType::token_op_logic_and) || matchToken(TokenType::token_op_logic_or))
        {
            consumeToken({TokenType::token_op_logic_and, TokenType::token_op_logic_or});

            FungOpType cond_op = (getPrevious().type == TokenType::token_op_logic_and)
                ? FungOpType::fung_op_logic_and
                : FungOpType::fung_op_logic_or;
            
            auto rhs = parseComparison();

            lhs = std::make_unique<fung::syntax::BinaryExpr>(std::move(lhs), std::move(rhs), cond_op);
        }

        return lhs;
    }

    // std::unique_ptr<fung::syntax::IStmt> parseUse();
    // std::unique_ptr<fung::syntax::IStmt> parseVar();
    // std::unique_ptr<fung::syntax::IStmt> parseParam();
    // std::unique_ptr<fung::syntax::IStmt> parseFunc();
    // std::unique_ptr<fung::syntax::IStmt> parseField();
    // std::unique_ptr<fung::syntax::IStmt> parseObject();
    // std::unique_ptr<fung::syntax::IStmt> parseAssign();
    // std::unique_ptr<fung::syntax::IStmt> parseReturn();
    // std::unique_ptr<fung::syntax::IStmt> parseIf();
    // std::unique_ptr<fung::syntax::IStmt> parseElse();
    // std::unique_ptr<fung::syntax::IStmt> parseBlock();
    // std::unique_ptr<fung::syntax::IStmt> parseSubStmt();
    // std::unique_ptr<fung::syntax::IStmt> parseStmt();

    /* Parser public impl. */
}
