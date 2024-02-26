#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <memory>
#include <variant>
#include "frontend/token.hpp"
#include "syntax/expressions.hpp"
#include "syntax/stmtbase.hpp"

// TODO: finish statement nodes!!

namespace fung::syntax
{
    enum NestedExprType
    {
        nested_expr_call,
        nested_expr_element,
        nested_expr_access,
        nested_expr_unary,
        nested_expr_binary
    };

    class UseStmt : public IStmt
    {
    private:
        fung::frontend::Token identifier;
    public:
        UseStmt(const fung::frontend::Token& identifier_token);

        const fung::frontend::Token& getIdentifier() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class VarStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> right_expr;
        fung::frontend::Token identifier;
        bool immutable_flag;
    public:
        VarStmt(std::unique_ptr<IExpr> expr, const fung::frontend::Token& identifier_token, bool is_let);

        const std::unique_ptr<IExpr>& getRXpr() const;
        const fung::frontend::Token& getIdentifier() const;
        bool isImmutable() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ParamDecl : public IStmt
    {
    private:
        fung::frontend::Token identifier;
        bool value_flag;
    public:
        ParamDecl(const fung::frontend::Token& idenfitier_token, bool is_value);

        const fung::frontend::Token& getIdentifier() const;
        bool isValue() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class FuncDecl : public IStmt
    {
    private:
        BlockStmt body;
        std::vector<ParamDecl> params;
        fung::frontend::Token name;

    public:
        FuncDecl(const fung::frontend::Token& name_token);

        void addParam(const ParamDecl& param);
        void addBodyStmt(std::unique_ptr<IStmt> stmt_ptr);

        const BlockStmt& getBodyBlock() const;
        const std::vector<ParamDecl>& getParams() const;
        const fung::frontend::Token& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class FieldDecl : public IStmt
    {
    private:
        fung::frontend::Token name;
    
    public:
        FieldDecl(const fung::frontend::Token& field_name);

        const fung::frontend::Token& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ObjectDecl : public IStmt
    {
    private:
        std::vector<FieldDecl> fields;
        fung::frontend::Token type_name;
    public:
        ObjectDecl(const fung::frontend::Token& name);

        const std::vector<FieldDecl> getFields() const;
        void addField(const FieldDecl& field);
        const fung::frontend::Token& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class AssignStmt : public IStmt
    {
    private:
        AccessExpr var_lvalue;
        std::unique_ptr<IExpr> var_rvalue;
    public:
        AssignStmt(AccessExpr lvalue, std::unique_ptr<IExpr> rvalue);

        const AccessExpr& getLValue() const;
        const std::unique_ptr<IExpr>& getRValue() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ReturnStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> result;
    public:
        ReturnStmt(std::unique_ptr<IExpr> result_expr);

        const std::unique_ptr<IExpr>& getResult() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class IfStmt : public IStmt
    {
    private:
        BlockStmt body;
        std::unique_ptr<IExpr> conditional;
        std::unique_ptr<IStmt> other;

    public:
        IfStmt(BlockStmt block_stmt, std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> other_stmt);

        const BlockStmt& getBody() const;
        const std::unique_ptr<IExpr>& getConditional() const;
        const std::unique_ptr<IStmt>& getOtherElse() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ElseStmt : public IStmt
    {
    private:
        BlockStmt body;
    public:
        ElseStmt();

        const BlockStmt& getBody() const;
        void addStmt(std::unique_ptr<IStmt> stmt);

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class WhileStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> conditional;
        BlockStmt body;
    public:
        WhileStmt(std::unique_ptr<IExpr> conditional_expr);

        const std::unique_ptr<IExpr>& getConditional() const;
        const BlockStmt& getBody() const;
        void addStmt(std::unique_ptr<IStmt> stmt);

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class BlockStmt : public IStmt
    {
    private:
        std::vector<std::unique_ptr<IStmt>> body;
    public:
        BlockStmt();

        const std::vector<std::unique_ptr<IStmt>>& getBody() const;
        void addStmt(std::unique_ptr<IStmt> stmt);

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };
}

#endif