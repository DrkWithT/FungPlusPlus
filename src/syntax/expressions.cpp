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

    CallExpr::CallExpr(const std::string& token)
    : args {}, identifier(std::move(token)) {}

    const std::string& CallExpr::getIdentifier() const
    {
        return identifier;
    }

    void CallExpr::addArgument(std::unique_ptr<IExpr> arg)
    {
        args.emplace_back(std::move(arg));
    }

    const std::vector<std::unique_ptr<IExpr>>& CallExpr::getArguments() const
    {
        return args;
    }

    std::any CallExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitCallExpr(*this);
    }

    /* ElementExpr impl. */

    ElementExpr::ElementExpr(std::any content_box, FungSimpleType element_type)
    : content(std::move(content_box)), type {element_type} {}

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

    AccessExpr::AccessExpr(std::string& left_name, std::vector<std::unique_ptr<IExpr>> key_vec)
    : keys(std::move(key_vec)), lvalue(std::move(left_name)) {}

    const std::vector<std::unique_ptr<IExpr>>& AccessExpr::getKeys() const
    {
        return keys;
    }

    const std::string& AccessExpr::getLvalueName() const
    {
        return lvalue;
    }

    std::any AccessExpr::accept(ExprVisitor<std::any>& visitor)
    {
        return visitor.visitAccessExpr(*this);
    }

    /* UnaryExpr impl. */

    UnaryExpr::UnaryExpr(std::unique_ptr<IExpr> inner_expr, FungOperatorType op_type)
    : inner(std::move(inner_expr)), op {op_type} {}

    const std::unique_ptr<IExpr>& UnaryExpr::getInnerExpr() const
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

    BinaryExpr::BinaryExpr(std::unique_ptr<IExpr> left_binexpr, std::unique_ptr<IExpr> right_binexpr, FungOperatorType op_symbol)
    : left(std::move(left_binexpr)), right(std::move(right_binexpr)), op {op_symbol}, nests_unaries {true} {}

    BinaryExpr::BinaryExpr(std::unique_ptr<IExpr> left_binexpr, std::unique_ptr<IExpr> right_binexpr, FungOperatorType op_symbol)
    : left(std::move(left_binexpr)), right(std::move(right_binexpr)), op {op_symbol}, nests_unaries {false} {}

    const std::unique_ptr<IExpr>& BinaryExpr::getLeftExpr() const
    {
        return left;
    }

    const std::unique_ptr<IExpr>& BinaryExpr::getRightExpr() const
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
