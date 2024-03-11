/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements recursive descent parser for Fung.
 * @note See TODOs e.g add checks for early right parens, brackets, and more so that trailing commas are prevented.
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <sstream>
#include <utility>
#include "frontend/parser.hpp"

namespace fung::frontend
{
    using FungLiteralType = fung::syntax::FungSimpleType;
    using FungOpType = fung::syntax::FungOperatorType;

    static const char* keyword_use = "use";
    static const char* keyword_let = "let";
    static const char* keyword_mut = "mut";
    static const char* keyword_val = "val";
    static const char* keyword_ref = "ref";
    static const char* keyword_fun = "fun";
    static const char* keyword_field = "field";
    static const char* keyword_object = "object";
    static const char* keyword_end = "end";
    static const char* keyword_ret = "ret";
    static const char* keyword_if = "if";
    static const char* keyword_else = "else";
    static const char* keyword_while = "while";

    /* ProgramUnit impl. */

    ProgramUnit::ProgramUnit(const std::string& unit_name)
    : statements {}, name {unit_name} {}

    const std::vector<std::unique_ptr<fung::syntax::IStmt>>& ProgramUnit::getStatements() const
    {
        return statements;
    }

    void ProgramUnit::appendStmt(std::unique_ptr<fung::syntax::IStmt> stmt)
    {
        statements.emplace_back(std::move(stmt));
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
                return;
            }
        }

        reportError(getCurrent(), "Unexpected token: ");
        throw std::runtime_error {""};
    }

    void Parser::reportError(const Token& token, const std::string& msg_header)
    {
        std::cerr << msg_header << "Token {begin = " << token.begin << ", type = " << token.type << "}\n";
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

    std::unique_ptr<fung::syntax::IExpr> Parser::parseListLiteral()
    {
        consumeToken({TokenType::token_lbrack});

        std::vector<std::unique_ptr<fung::syntax::IExpr>> literal_args {};

        while (!matchToken(TokenType::token_rbrack))
        {
            auto temp_arg = parseElement();
            literal_args.emplace_back(std::move(temp_arg));

            /// TODO: add check to stop early on rbrack.
            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rbrack});

        return std::make_unique<fung::syntax::ElementExpr>(std::move(literal_args), FungLiteralType::fung_simple_type_list);
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseObjectLiteral()
    {
        consumeToken({TokenType::token_lbrace});

        std::vector<std::unique_ptr<fung::syntax::IExpr>> object_args {};

        while (!matchToken(TokenType::token_rbrace))
        {
            auto temp_arg = parseElement();
            object_args.emplace_back(std::move(temp_arg));

            /// TODO: add check to stop early on rbrace.
            consumeToken({TokenType::token_comma});
        }

        return std::make_unique<fung::syntax::ElementExpr>(std::move(object_args), FungLiteralType::fung_simple_type_object);
    }

    std::unique_ptr<fung::syntax::IExpr> Parser::parseElement()
    {
        TokenType temp_type = getCurrent().type;
        if (matchToken(TokenType::token_special_nil))
        {
            auto value_nil = std::make_unique<fung::syntax::ElementExpr>(nullptr, FungLiteralType::fung_simple_type_nil);

            consumeToken({TokenType::token_special_nil});
            return value_nil;
        }
        else if (matchToken(TokenType::token_special_true) || matchToken(TokenType::token_special_false))
        {
            auto value_bool = std::make_unique<fung::syntax::ElementExpr>((temp_type == token_special_true), FungLiteralType::fung_simple_type_bool);

            consumeToken({TokenType::token_special_true, TokenType::token_special_false});
            return value_bool;
        }
        else if (matchToken(TokenType::token_integer))
        {
            int x = std::stoi(stringifyTokenFully(getCurrent(), source_viewer));

            auto value_int = std::make_unique<fung::syntax::ElementExpr>(x, FungLiteralType::fung_simple_type_int);

            consumeToken({TokenType::token_integer});
            return value_int;
        }
        else if (matchToken(TokenType::token_float))
        {
            double x = std::stod(stringifyTokenFully(getCurrent(), source_viewer));

            auto value_float = std::make_unique<fung::syntax::ElementExpr>(x, FungLiteralType::fung_simple_type_float);

            consumeToken({TokenType::token_float});
            return value_float;
        }
        else if (matchToken(TokenType::token_string))
        {
            auto value_str = std::make_unique<fung::syntax::ElementExpr>(stringifyTokenFully(getCurrent(), source_viewer), FungLiteralType::fung_simple_type_string);

            consumeToken({TokenType::token_string});
            return value_str;
        }
        else if (matchToken(TokenType::token_lbrack))
        {
            return parseListLiteral();
        }
        else if (matchToken(TokenType::token_lbrace))
        {
            return parseObjectLiteral();
        }
        else if (matchToken(TokenType::token_lparen))
        {
            consumeToken({TokenType::token_lparen});
            auto top_expr = parseConditional();
            consumeToken({TokenType::token_rparen});

            return top_expr;
        }
        else if (matchToken(TokenType::token_op_extract))
        {
            consumeToken({TokenType::token_op_extract});
            return parseCall();
        }

        reportError(getCurrent(), "Unexpected token of elemental expr: ");
        throw std::runtime_error {""};
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

            /// TODO: add check to stop early on rparen.
            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rparen});

        fung::syntax::CallExpr fn_call {function_name};

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

        /// NOTE: Since access expressions can also be for callees by name, skip field accesses (possible key checks) in that case...
        if (matchToken(TokenType::token_lparen))
        {
            return std::make_unique<fung::syntax::AccessExpr>(lvalue_name, std::move(keys));
        }

        consumeToken({TokenType::token_lbrack});

        while (!matchToken(TokenType::token_rbrack))
        {
            auto keying_expr = parseElement();
            keys.emplace_back(std::move(keying_expr));

            /// TODO: add check to stop early on rbrack.
            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rbrack});

        return std::make_unique<fung::syntax::AccessExpr>(lvalue_name, std::move(keys));
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

            return std::make_unique<fung::syntax::UnaryExpr>(std::move(inner_element), op_type);
        }

        auto inner_access = parseAccess();

        return std::make_unique<fung::syntax::UnaryExpr>(std::move(inner_access), op_type);
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

    std::unique_ptr<fung::syntax::IStmt> Parser::parseUse()
    {
        consumeToken({TokenType::token_keyword});

        if (stringifyToken(getPrevious(), source_viewer) == keyword_use)
        {
            std::string use_name = stringifyTokenFully(getCurrent(), source_viewer);

            consumeToken({TokenType::token_identifier});
            return std::make_unique<fung::syntax::UseStmt>(use_name);
        }

        reportError(getPrevious(), "Invalid keyword in use: ");
        throw std::runtime_error {""};
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseVar()
    {
        consumeToken({TokenType::token_keyword});

        bool var_is_let;

        if (stringifyToken(getPrevious(), source_viewer) == keyword_let)
        {
            var_is_let = true;
        }
        else if (stringifyToken(getPrevious(), source_viewer) == keyword_mut)
        {
            var_is_let = false;
        }
        else
        {
            reportError(getPrevious(), "Invalid keyword for var decl: ");
            throw std::runtime_error {""};
        }

        consumeToken({TokenType::token_identifier});

        std::string var_name = stringifyTokenFully(getPrevious(), source_viewer);

        consumeToken({TokenType::token_op_assign});

        auto rvalue = parseConditional();

        return std::make_unique<fung::syntax::VarStmt>(std::move(rvalue), var_name, var_is_let);
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseParam()
    {
        consumeToken({TokenType::token_keyword});

        bool param_is_val;

        if (stringifyToken(getPrevious(), source_viewer) == keyword_val)
        {
            param_is_val = true;
        }
        else if (stringifyToken(getPrevious(), source_viewer) == keyword_ref)
        {
            param_is_val = false;
        }
        else
        {
            reportError(getPrevious(), "Invalid keyword for function param: ");
            throw std::runtime_error {""};
        }

        consumeToken({TokenType::token_identifier});

        std::string param_name = stringifyTokenFully(getPrevious(), source_viewer);

        return std::make_unique<fung::syntax::ParamDecl>(param_name, param_is_val);
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseFunc()
    {
        consumeToken({TokenType::token_keyword});

        std::string callee_name;

        if (stringifyToken(getPrevious(), source_viewer) == keyword_fun)
        {
            callee_name = stringifyToken(getCurrent(), source_viewer);
        }
        else
        {
            reportError(getPrevious(), "Invalid keyword for function decl: ");
            throw std::runtime_error {""};
        }

        consumeToken({TokenType::token_identifier});
        consumeToken({TokenType::token_lparen});

        std::vector<std::unique_ptr<fung::syntax::IStmt>> callee_params {};

        while (!matchToken(TokenType::token_rparen))
        {
            auto temp_param = parseParam();
            callee_params.emplace_back(std::move(temp_param));

            /// TODO: add check to stop early on rparen.
            consumeToken({TokenType::token_comma});
        }

        consumeToken({TokenType::token_rparen});

        auto callee_body = parseBlock();

        return std::make_unique<fung::syntax::FuncDecl>(std::move(callee_body), std::move(callee_params), callee_name);
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseField()
    {
        consumeToken({TokenType::token_keyword});

        std::string field_name;

        if (stringifyToken(getPrevious(), source_viewer) == keyword_field)
        {
            field_name = stringifyTokenFully(getCurrent(), source_viewer);
        }
        else
        {
            reportError(getPrevious(), "Invalid keyword for object field decl: ");
            throw std::runtime_error {""};
        }

        consumeToken({TokenType::token_identifier});

        return std::make_unique<fung::syntax::FieldDecl>(field_name);
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseObject()
    {
        consumeToken({TokenType::token_keyword});

        std::string obj_type_name;

        if (stringifyToken(getPrevious(), source_viewer) == keyword_object)
        {
            obj_type_name = stringifyTokenFully(getCurrent(), source_viewer);
        }
        else
        {
            reportError(getPrevious(), "Invalid keyword for object decl: ");
            throw std::runtime_error {""};
        }

        consumeToken({TokenType::token_identifier});

        std::vector<std::unique_ptr<fung::syntax::IStmt>> temp_fields {};

        while (stringifyToken(getCurrent(), source_viewer) != keyword_end)
        {
            auto obj_field = parseField();
            temp_fields.emplace_back(std::move(obj_field));
        }

        consumeToken({TokenType::token_keyword});

        return std::make_unique<fung::syntax::ObjectDecl>(std::move(temp_fields), obj_type_name);
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseAssignOrExprStmt()
    {
        // Advance past identifier, but keep its lexeme for either case: variable assign or named expr-stmt of a call!
        auto name_access = parseAccess();

        if (matchToken(TokenType::token_op_assign))
        {
            consumeToken({TokenType::token_op_assign});
            auto rhs = parseConditional();

            return std::make_unique<fung::syntax::AssignStmt>(std::move(name_access), std::move(rhs));
        }
        else if (matchToken(TokenType::token_lparen))
        {
            consumeToken({TokenType::token_lparen});

            std::vector<std::unique_ptr<fung::syntax::IExpr>> function_args {};

            while (!matchToken(TokenType::token_rparen))
            {
                auto temp_arg = parseElement();
                function_args.emplace_back(std::move(temp_arg));

                /// TODO: add check to stop early on rparen.
                consumeToken({TokenType::token_comma});
            }

            consumeToken({TokenType::token_rparen});

            return std::make_unique<fung::syntax::ExprStmt>(std::move(function_args), std::move(name_access));
        }

        reportError(getCurrent(), "Unknown statement at: ");
        throw std::runtime_error {""};
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseReturn()
    {
        consumeToken({TokenType::token_keyword});

        if (stringifyToken(getPrevious(), source_viewer) != keyword_ret)
        {
            reportError(getPrevious(), "Invalid keyword for return stmt: ");
            throw std::runtime_error {""};
        }

        auto result_expr = parseConditional();

        return std::make_unique<fung::syntax::ReturnStmt>(std::move(result_expr));
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseIf()
    {
        consumeToken({TokenType::token_keyword});

        if (stringifyToken(getPrevious(), source_viewer) != keyword_if)
        {
            reportError(getPrevious(), "Invalid keyword for if stmt: ");
            throw std::runtime_error {""};
        }

        auto conditional_expr = parseConditional();
        std::vector<std::unique_ptr<fung::syntax::IStmt>> stmts {};
        bool has_else = false;

        while (!matchToken(TokenType::token_eof))
        {
            auto inner_stmt = parseSubStmt();
            stmts.emplace_back(std::move(inner_stmt));

            if (stringifyToken(getCurrent(), source_viewer) == keyword_end)
            {
                has_else = false;
                break;
            }

            if (stringifyToken(getCurrent(), source_viewer) == keyword_else)
            {
                has_else = true;
                break;
            }
        }
        
        auto if_block = std::make_unique<fung::syntax::BlockStmt>(std::move(stmts));

        if (has_else)
        {
            auto other_else = parseElse();

            return std::make_unique<fung::syntax::IfStmt>(std::move(conditional_expr), std::move(if_block), std::move(other_else));
        }

        consumeToken({TokenType::token_keyword});

        return std::make_unique<fung::syntax::IfStmt>(std::move(conditional_expr), std::move(if_block));
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseElse()
    {
        consumeToken({TokenType::token_keyword});

        if (stringifyToken(getPrevious(), source_viewer) != keyword_else)
        {
            reportError(getPrevious(), "Invalid keyword for else stmt: ");
            throw std::runtime_error {""};
        }

        auto block = parseBlock();

        return std::make_unique<fung::syntax::ElseStmt>(std::move(block));
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseWhile()
    {
        consumeToken({TokenType::token_keyword});

        if (stringifyToken(getPrevious(), source_viewer) != keyword_while)
        {
            reportError(getPrevious(), "Invalid keyword for while stmt: ");
            throw std::runtime_error {""};
        }

        auto loop_conditions = parseConditional();
        auto loop_body = parseBlock();

        return std::make_unique<fung::syntax::WhileStmt>(std::move(loop_conditions), std::move(loop_body));
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseSubStmt()
    {
        if (matchToken(token_comment))
        {
            consumeToken({});
            return nullptr;
        }
        else if (matchToken(TokenType::token_identifier))
        {
            return parseAssignOrExprStmt();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_let || stringifyToken(getCurrent(), source_viewer) == keyword_mut)
        {
            return parseVar();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_if)
        {
            return parseIf();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_while)
        {
            return parseWhile();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_ret)
        {
            return parseReturn();
        }

        reportError(getCurrent(), "Unexpected token for block-lvl statement: ");
        throw std::runtime_error {""};
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseBlock()
    {
        std::vector<std::unique_ptr<fung::syntax::IStmt>> stmts {};

        while (!matchToken(TokenType::token_eof))
        {
            if (stringifyToken(getCurrent(), source_viewer) == keyword_end)
            {
                break;
            }

            auto inner_stmt = parseSubStmt();
            stmts.emplace_back(std::move(inner_stmt));
        }

        consumeToken({TokenType::token_keyword});

        return std::make_unique<fung::syntax::BlockStmt>(std::move(stmts));
    }

    std::unique_ptr<fung::syntax::IStmt> Parser::parseStmt()
    {
        if (matchToken(token_comment))
        {
            consumeToken({});
            return nullptr;
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_use)
        {
            return parseUse();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_let || stringifyToken(getCurrent(), source_viewer) == keyword_mut)
        {
            return parseVar();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_fun)
        {
            return parseFunc();
        }
        else if (stringifyToken(getCurrent(), source_viewer) == keyword_object)
        {
            return parseObject();
        }

        reportError(getCurrent(), "Unknown statement token at: ");
        throw std::runtime_error {""};
    }

    /* Parser public impl. */

    Parser::Parser(const char* source_cptr, size_t source_size)
    : lexer {source_cptr, source_size}, source_viewer {source_cptr}, previous {.type = TokenType::token_eof}, current {.type = TokenType::token_eof}, had_error {false} {}

    [[nodiscard]] bool Parser::parseFile(ProgramUnit& unit)
    {
        consumeToken({TokenType::token_eof});

        while (!matchToken(TokenType::token_eof))
        {
            try
            {
                auto temp_stmt = parseStmt();

                if (temp_stmt)
                {
                    unit.appendStmt(std::move(temp_stmt));
                }
            }
            catch (std::invalid_argument& parse_err)
            {
                had_error = true;
                synchronizeParse();
            }
        }

        return !had_error;
    }
}
