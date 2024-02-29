#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <memory>
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
        fung_simple_type_float,
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
        std::vector<std::unique_ptr<fung::syntax::IExpr>> args;
        std::string identifier;
    public:
        CallExpr(const std::string& lexeme);

        const std::string& getIdentifier() const;
        void addArgument(std::unique_ptr<fung::syntax::IExpr> arg);
        const std::vector<std::unique_ptr<fung::syntax::IExpr>>& getArguments() const;

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
        std::vector<std::unique_ptr<IExpr>> keys;
        std::string lvalue;
    public:
        AccessExpr(std::string& left_name);

        void addAccessKey(std::unique_ptr<IExpr> key_expr);
        const std::vector<std::unique_ptr<IExpr>>& getKeys() const;
        const std::string& getLvalueName() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    class UnaryExpr : public IExpr
    {
    private:
        std::unique_ptr<IExpr> inner; // AccessExpr!
        FungOperatorType op;
    public:
        UnaryExpr(std::unique_ptr<IExpr> inner_expr, FungOperatorType op_type);

        const std::unique_ptr<IExpr>& getInnerExpr() const;
        FungOperatorType getOperator() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };

    class BinaryExpr : public IExpr
    {
    private:
        std::unique_ptr<IExpr> left;
        std::unique_ptr<IExpr> right;
        FungOperatorType op;
        bool nests_unaries;
    public:
        BinaryExpr(std::unique_ptr<IExpr> left_binexpr, std::unique_ptr<IExpr> right_binexpr, FungOperatorType op_symbol);
        BinaryExpr(std::unique_ptr<IExpr> left_binexpr, std::unique_ptr<IExpr> right_binexpr, FungOperatorType op_symbol);

        const std::unique_ptr<IExpr>& getLeftExpr() const;
        const std::unique_ptr<IExpr>& getRightExpr() const;
        FungOperatorType getOperator() const;
        [[nodiscard]] bool isNestingUnaries() const;

        std::any accept(ExprVisitor<std::any>& visitor) override;
    };
}

#endif