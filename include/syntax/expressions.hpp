#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <variant>
#include <vector>
#include "frontend/token.hpp"
#include "syntax/exprbase.hpp"

namespace fung::syntax
{
    /// @note Denotes literals.
    enum FungSimpleType
    {
        fung_simple_type_nil,
        fung_simple_type_bool,
        fung_simple_type_int,
        fung_simple_type_string,
        fung_simple_type_list,
        fung_simple_type_object
    };

    /// @note For now, the token for '=' is called an operator although it's actually required punctuation for assignment. Thus no `fung_op_assign` exists so far.
    enum FungOperatorType
    {
        fung_op_nonil,
        fung_op_plus,
        fung_op_minus,
        fung_op_times,
        fung_op_slash,
        fung_op_isequal,
        fung_op_unequal,
        fung_op_lt,
        fung_op_gt,
        fung_op_lte,
        fung_op_gte,
        fung_op_logic_and,
        fung_op_logic_or
    };

    class CallExpr : public IExpr
    {
    private:
        std::vector<ElementExpr> args;
        fung::frontend::Token identifier;
    public:
        CallExpr(fung::frontend::Token& token);

        const fung::frontend::Token& getIdentifierToken() const;
        void addArgument(const ElementExpr& arg);
        const std::vector<ElementExpr>& getArguments() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    /// @todo Maybe add move methods for less copying in vector emplacement?
    class ElementExpr : public IExpr
    {
    private:
        std::any content;
        FungSimpleType type;
    public:
        ElementExpr(std::any content_box, FungSimpleType element_type);

        const std::any& getContent() const;
        FungSimpleType getType() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    class AccessExpr : public IExpr
    {
    private:
        std::vector<ElementExpr> keys;
        std::variant<fung::frontend::Token, CallExpr> lvalue;
    public:
        AccessExpr(fung::frontend::Token& token);
        AccessExpr(CallExpr& call_expr);

        void addAccessKey(ElementExpr& key_expr);
        const std::vector<ElementExpr>& getKeys() const;
        const std::variant<fung::frontend::Token, CallExpr>& getLvalueVariant() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    class UnaryExpr : public IExpr
    {
    private:
        AccessExpr inner;
        FungOperatorType op;
    public:
        UnaryExpr(AccessExpr& inner_expr,         FungOperatorType op_type);

        const AccessExpr& getInnerExpr() const;
        FungOperatorType getOperator() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    class BinaryExpr : public IExpr
    {
    private:
        std::any left;
        std::any right;
        FungOperatorType op;
        bool nests_unaries;
    public:
        BinaryExpr(UnaryExpr left_expr, UnaryExpr right_expr, FungOperatorType op_symbol);
        BinaryExpr(BinaryExpr left_expr, BinaryExpr right_expr, FungOperatorType op_symbol);

        const std::any& getLeftExpr() const;
        const std::any& getRightExpr() const;
        FungOperatorType getOperator() const;
        [[nodiscard]] bool isNestingUnaries() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };
}

#endif