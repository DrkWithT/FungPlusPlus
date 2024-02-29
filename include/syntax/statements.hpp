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
        std::string identifier;
    public:
        UseStmt(std::string& identifier_lexeme);

        const std::string& getIdentifier() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class VarStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> right_expr;
        std::string identifier;
        bool immutable_flag;
    public:
        VarStmt(std::unique_ptr<IExpr> expr, std::string& identifier_lexeme, bool is_let);

        const std::unique_ptr<IExpr>& getRXpr() const;
        const std::string& getIdentifier() const;
        bool isImmutable() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ParamDecl : public IStmt
    {
    private:
        std::string identifier;
        bool value_flag;
    public:
        ParamDecl(std::string& idenfitier_lexeme, bool is_value);

        const std::string& getIdentifier() const;
        bool isValue() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class FuncDecl : public IStmt
    {
    private:
        BlockStmt body;
        std::vector<ParamDecl> params;
        std::string name;

    public:
        FuncDecl(std::string& name_lexeme);

        void addParam(const ParamDecl& param);
        void addBodyStmt(std::unique_ptr<IStmt> stmt_ptr);

        const BlockStmt& getBodyBlock() const;
        const std::vector<ParamDecl>& getParams() const;
        const std::string& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class FieldDecl : public IStmt
    {
    private:
        std::string name;
    
    public:
        FieldDecl(std::string& field_name);

        const std::string& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ObjectDecl : public IStmt
    {
    private:
        std::vector<FieldDecl> fields;
        std::string type_name;
    public:
        ObjectDecl(std::string& name_lexeme);

        const std::vector<FieldDecl> getFields() const;
        void addField(const FieldDecl& field);
        const std::string& getName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class AssignStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> var_lvalue;
        std::unique_ptr<IExpr> var_rvalue;
    public:
        AssignStmt(std::unique_ptr<IExpr> lvalue, std::unique_ptr<IExpr> rvalue);

        const std::unique_ptr<IExpr>& getLValue() const;
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
        std::unique_ptr<IExpr> conditional;
        std::unique_ptr<IStmt> body;
        std::unique_ptr<IStmt> other;

    public:
        IfStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> block_stmt, std::unique_ptr<IStmt> other_stmt);

        const std::unique_ptr<IExpr>& getConditional() const;
        const std::unique_ptr<IStmt>& getBody() const;
        const std::unique_ptr<IStmt>& getOtherElse() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ElseStmt : public IStmt
    {
    private:
        std::unique_ptr<IStmt> body;
    public:
        ElseStmt(std::unique_ptr<IStmt> body_block);

        const std::unique_ptr<IStmt>& getBody() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class ExprStmt : public IStmt
    {
    private:
        std::string name;
    public:
        ExprStmt(std::string& callee_name);

        const std::string& getCalleeName() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };

    class WhileStmt : public IStmt
    {
    private:
        std::unique_ptr<IExpr> conditional;
        std::unique_ptr<IStmt> body;
    public:
        WhileStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> body_stmt);

        const std::unique_ptr<IExpr>& getConditional() const;
        const std::unique_ptr<IStmt>& getBody() const;

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