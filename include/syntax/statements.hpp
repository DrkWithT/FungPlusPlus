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
        std::unique_ptr<IStmt> body;
        std::vector<std::unique_ptr<IStmt>> params;
        std::string name;

    public:
        FuncDecl(std::unique_ptr<IStmt> body_stmt, std::vector<std::unique_ptr<IStmt>> param_list, std::string& name_lexeme);

        void addParam(const std::unique_ptr<IStmt>& param_decl);

        const std::unique_ptr<IStmt>& getBodyBlock() const;
        const std::vector<std::unique_ptr<IStmt>>& getParams() const;
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
        std::vector<std::unique_ptr<IStmt>> fields;
        std::string type_name;
    public:
        ObjectDecl(std::vector<std::unique_ptr<IStmt>> field_list, std::string& name_lexeme);

        const std::vector<std::unique_ptr<IStmt>>& getFields() const;
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
        IfStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> block_stmt);

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
        std::vector<std::unique_ptr<fung::syntax::IExpr>> arg_list;
        std::unique_ptr<fung::syntax::IExpr> accessor;
    public:
        ExprStmt(std::vector<std::unique_ptr<fung::syntax::IExpr>> args, std::unique_ptr<fung::syntax::IExpr> access_expr);

        const std::vector<std::unique_ptr<fung::syntax::IExpr>>& getArgList() const;
        const std::unique_ptr<fung::syntax::IExpr>& getAccessor() const;

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
        BlockStmt(std::vector<std::unique_ptr<IStmt>> body_stmts);

        const std::vector<std::unique_ptr<IStmt>>& getBody() const;

        virtual std::any accept(StmtVisitor<std::any>& visitor) override;
    };
}

#endif