/**
 * @file expressions.cpp
 * @author DrkWithT
 * @brief Implements expression AST nodes.
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "syntax/expressions.hpp"
#include "frontend/token.hpp"

using FungToken = fung::frontend::Token;

namespace fung::syntax
{
    /* CallExpr impl. */

    CallExpr::CallExpr(FungToken& token)
    : args {}, identifier {token}
    {}

    const FungToken& CallExpr::getIdentifierToken() const
    {
        return identifier;
    }
    
    void CallExpr::addArgument(const ElementExpr& arg)
    {
        args.emplace_back(arg);
    }
    
    const std::vector<ElementExpr>& CallExpr::getArguments() const
    {
        return args;
    }

    std::any CallExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitCallExpr(*this);
    }

    /* ElementExpr impl. */

    ElementExpr::ElementExpr(std::any content_box, FungSimpleType element_type)
    : content(std::move(content_box)), type {element_type}
    {}

    const std::any& ElementExpr::getContent() const
    {
        return content;
    }

    FungSimpleType ElementExpr::getType() const
    {
        return type;
    }

    std::any ElementExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitElementExpr(*this);
    }

    /* AccessExpr impl. */

    AccessExpr::AccessExpr(FungToken& token)
    : keys {}, lvalue {}
    {
        lvalue = token;
    }

    AccessExpr::AccessExpr(CallExpr& call_expr)
    : keys {}, lvalue {}
    {
        lvalue = call_expr;
    }

    void AccessExpr::addAccessKey(ElementExpr& key_expr)
    {
        keys.emplace_back(key_expr);
    }

    const std::vector<ElementExpr>& AccessExpr::getKeys() const
    {
        return keys;
    }

    const std::variant<FungToken, CallExpr>& AccessExpr::getLvalueVariant() const
    {
        return lvalue;
    }

    std::any AccessExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitAccessExpr(*this);
    }

    /* UnaryExpr impl. */

    UnaryExpr::UnaryExpr(AccessExpr& inner_expr, FungOperatorType op_type)
    : inner {inner_expr}, op {op_type}
    {}

    const AccessExpr& UnaryExpr::getInnerExpr() const
    {
        return inner;
    }

    FungOperatorType UnaryExpr::getOperator() const
    {
        return op;
    }

    std::any UnaryExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitUnaryExpr(*this);
    }

    /* BinaryExpr impl. */

    BinaryExpr::BinaryExpr(UnaryExpr left_expr, UnaryExpr right_expr, FungOperatorType op_symbol)
    : left {}, right {}, op {op_symbol}, nests_unaries {true}
    {
        left = left_expr;
        right = right_expr;
    }

    BinaryExpr::BinaryExpr(BinaryExpr left_expr, BinaryExpr right_expr, FungOperatorType op_symbol)
    : left {}, right {}, op {op_symbol}, nests_unaries {false}
    {
        left = left_expr;
        right = right_expr;
    }

    const std::any& BinaryExpr::getLeftExpr() const
    {
        return left;
    }

    const std::any& BinaryExpr::getRightExpr() const
    {
        return right;
    }

    FungOperatorType BinaryExpr::getOperator() const
    {
        return op;
    }

    [[nodiscard]] bool BinaryExpr::isNestingUnaries() const
    {
        return nests_unaries;
    }

    std::any BinaryExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitBinaryExpr(*this);
    }
}
