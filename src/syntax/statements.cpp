/**
 * @file statements.cpp
 * @author DrkWithT
 * @brief Implements AST statement nodes.
 * @date 2024-02-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "syntax/statements.hpp"

namespace fung::syntax
{
    /* UseStmt impl */

    UseStmt::UseStmt(std::string& identifier_lexeme)
    : identifier(std::move(identifier_lexeme)) {}

    const std::string& UseStmt::getIdentifier() const
    {
        return identifier;
    }

    std::any UseStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitUseStmt(*this);
    }

    /* VarStmt impl. */

    VarStmt::VarStmt(std::unique_ptr<IExpr> expr, std::string& identifier_lexeme, bool is_let)
    : right_expr(std::move(expr)), identifier(std::move(identifier_lexeme)), immutable_flag {is_let} {}

    const std::unique_ptr<IExpr>& VarStmt::getRXpr() const
    {
        return right_expr;
    }
    
    const std::string& VarStmt::getIdentifier() const
    {
        return identifier;
    }
    
    bool VarStmt::isImmutable() const
    {
        return immutable_flag;
    }

    std::any VarStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitVarStmt(*this);
    }

    /* ParamDecl impl. */

    ParamDecl::ParamDecl(std::string& idenfitier_lexeme, bool is_value)
    : identifier(std::move(idenfitier_lexeme)), value_flag {is_value} {}

    const std::string& ParamDecl::getIdentifier() const
    {
        return identifier;
    }

    bool ParamDecl::isValue() const
    {
        return value_flag;
    }

    std::any ParamDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitParamDecl(*this);
    }

    /* FuncDecl impl. */

    FuncDecl::FuncDecl(std::unique_ptr<IStmt> body_stmt, std::vector<std::unique_ptr<IStmt>> param_list, std::string& name_lexeme)
    : body(std::move(body_stmt)), params(std::move(param_list)), name(std::move(name_lexeme)) {}

    void FuncDecl::addParam(std::unique_ptr<IStmt> param_decl)
    {
        params.emplace_back(std::move(param_decl));
    }

    const std::unique_ptr<IStmt>& FuncDecl::getBodyBlock() const
    {
        return body;
    }

    const std::vector<std::unique_ptr<IStmt>>& FuncDecl::getParams() const
    {
        return params;
    }

    const std::string& FuncDecl::getName() const
    {
        return name;
    }

    std::any FuncDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitFuncDecl(*this);
    }

    /* FieldDecl impl. */

    FieldDecl::FieldDecl(std::string& field_name)
    : name(std::move(field_name)) {}

    const std::string& FieldDecl::getName() const
    {
        return name;
    }

    std::any FieldDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitFieldDecl(*this);
    }

    /* ObjectDecl */

    ObjectDecl::ObjectDecl(std::vector<std::unique_ptr<IStmt>> field_list, std::string& name_lexeme)
    : fields(std::move(field_list)), type_name(std::move(name_lexeme)) {}

    const std::vector<std::unique_ptr<IStmt>>& ObjectDecl::getFields() const
    {
        return fields;
    }

    const std::string& ObjectDecl::getName() const
    {
        return type_name;
    }

    std::any ObjectDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitObjectDecl(*this);
    }

    /* AssignStmt impl. */

    AssignStmt::AssignStmt(std::unique_ptr<fung::syntax::IExpr> lvalue, std::unique_ptr<IExpr> rvalue)
    : var_lvalue(std::move(lvalue)), var_rvalue(std::move(rvalue)) {}

    const std::unique_ptr<fung::syntax::IExpr>& AssignStmt::getLValue() const
    {
        return var_lvalue;
    }

    const std::unique_ptr<fung::syntax::IExpr>& AssignStmt::getRValue() const
    {
        return var_rvalue;
    }

    std::any AssignStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitAssignStmt(*this);
    }

    /* ReturnStmt impl. */

    ReturnStmt::ReturnStmt(std::unique_ptr<IExpr> result_expr)
    : result(std::move(result_expr))
    {}

    const std::unique_ptr<IExpr>& ReturnStmt::getResult() const
    {
        return result;
    }

    std::any ReturnStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitReturnStmt(*this);
    }

    /* IfStmt impl. */

    IfStmt::IfStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> block_stmt, std::unique_ptr<IStmt> other_stmt)
    : conditional(std::move(conditional_expr)), body(std::move(block_stmt)), other(std::move(other_stmt)) {}

    IfStmt::IfStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> block_stmt)
    : conditional(std::move(conditional_expr)), body(std::move(block_stmt)), other {} {}

    const std::unique_ptr<IExpr>& IfStmt::getConditional() const
    {
        return conditional;
    }

    const std::unique_ptr<IStmt>& IfStmt::getBody() const
    {
        return body;
    }

    const std::unique_ptr<IStmt>& IfStmt::getOtherElse() const
    {
        return other;
    }

    std::any IfStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitIfStmt(*this);
    }

    /* ElseStmt impl. */

    ElseStmt::ElseStmt(std::unique_ptr<IStmt> body_block)
    : body(std::move(body_block)) {}

    const std::unique_ptr<IStmt>& ElseStmt::getBody() const
    {
        return body;
    }

    std::any ElseStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitElseStmt(*this);
    }

    /* WhileStmt impl. */

    WhileStmt::WhileStmt(std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> body_stmt)
    : conditional(std::move(conditional_expr)), body(std::move(body_stmt)) {}

    const std::unique_ptr<IExpr>& WhileStmt::getConditional() const
    {
        return conditional;
    }

    const std::unique_ptr<IStmt>& WhileStmt::getBody() const
    {
        return body;
    }

    std::any WhileStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitWhileStmt(*this);
    }

    /* ExprStmt impl. */

    ExprStmt::ExprStmt(std::vector<std::unique_ptr<fung::syntax::IExpr>> args, std::unique_ptr<fung::syntax::IExpr> access_expr)
    : arg_list(std::move(args)), accessor(std::move(access_expr)) {}

    const std::vector<std::unique_ptr<fung::syntax::IExpr>>& ExprStmt::getArgList() const
    {
        return arg_list;
    }

    const std::unique_ptr<fung::syntax::IExpr>& ExprStmt::getAccessor() const
    {
        return accessor;
    }

    std::any ExprStmt::accept(StmtVisitor<std::any>& visitor)
    {
        visitor.visitExprStmt(*this);
    }

    /* BlockStmt impl. */

    BlockStmt::BlockStmt(std::vector<std::unique_ptr<IStmt>> body_stmts)
    : body(std::move(body_stmts)) {}

    const std::vector<std::unique_ptr<IStmt>>& BlockStmt::getBody() const
    {
        return body;
    }

    std::any BlockStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitBlockStmt(*this);
    }
}
