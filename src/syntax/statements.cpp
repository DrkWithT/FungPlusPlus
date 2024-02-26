/**
 * @file statements.cpp
 * @author DrkWithT
 * @brief Implements AST statement nodes.
 * @date 2024-02-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "syntax/statements.hpp"

namespace fung::syntax
{
    /* UseStmt impl */

    UseStmt::UseStmt(const fung::frontend::Token& identifier_token)
    : identifier {identifier_token}
    {}

    const fung::frontend::Token& UseStmt::getIdentifier() const
    {
        return identifier;
    }

    std::any UseStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitUseStmt(*this);
    }

    /* VarStmt impl. */

    VarStmt::VarStmt(std::unique_ptr<IExpr> expr, const fung::frontend::Token& identifier_token, bool is_let)
    : right_expr(std::move(expr)), identifier {identifier_token}, immutable_flag {is_let}
    {}

    const std::unique_ptr<IExpr>& VarStmt::getRXpr() const
    {
        return right_expr;
    }
    
    const fung::frontend::Token& VarStmt::getIdentifier() const
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

    ParamDecl::ParamDecl(const fung::frontend::Token& idenfitier_token, bool is_value)
    : identifier {idenfitier_token}, value_flag {is_value}
    {}

    const fung::frontend::Token& ParamDecl::getIdentifier() const
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
    FuncDecl::FuncDecl(const fung::frontend::Token& name_token)
    : body {}, params {}, name {name_token}
    {}

    void FuncDecl::addParam(const ParamDecl& param)
    {
        params.push_back(param);
    }

    void FuncDecl::addBodyStmt(std::unique_ptr<IStmt> stmt_ptr)
    {
        body.addStmt(std::move(stmt_ptr));
    }

    const BlockStmt& FuncDecl::getBodyBlock() const
    {
        return body;
    }
    
    const std::vector<ParamDecl>& FuncDecl::getParams() const
    {
        return params;
    }
    
    const fung::frontend::Token& FuncDecl::getName() const
    {
        return name;
    }

    std::any FuncDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitFuncDecl(*this);
    }

    /* FieldDecl impl. */

    FieldDecl::FieldDecl(const fung::frontend::Token& field_name)
    : name {field_name}
    {}

    const fung::frontend::Token& FieldDecl::getName() const
    {
        return name;
    }

    std::any FieldDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitFieldDecl(*this);
    }

    /* ObjectDecl */

    ObjectDecl::ObjectDecl(const fung::frontend::Token& name)
    : fields {}, type_name {name}
    {}

    const std::vector<FieldDecl> ObjectDecl::getFields() const
    {
        return fields;
    }
    
    void ObjectDecl::addField(const FieldDecl& field)
    {
        fields.push_back(field);
    }

    const fung::frontend::Token& ObjectDecl::getName() const
    {
        return type_name;
    }

    std::any ObjectDecl::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitObjectDecl(*this);
    }

    /* AssignStmt impl. */

    AssignStmt::AssignStmt(AccessExpr lvalue, std::unique_ptr<IExpr> rvalue)
    : var_lvalue {lvalue}, var_rvalue(std::move(rvalue))
    {}

    const AccessExpr& AssignStmt::getLValue() const
    {
        return var_lvalue;
    }

    const std::unique_ptr<IExpr>& AssignStmt::getRValue() const
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

    IfStmt::IfStmt(BlockStmt block_stmt, std::unique_ptr<IExpr> conditional_expr, std::unique_ptr<IStmt> other_stmt)
    : body {block_stmt}, conditional(std::move(conditional_expr)), other(std::move(other_stmt))
    {}

    const BlockStmt& IfStmt::getBody() const
    {
        return body;
    }

    const std::unique_ptr<IExpr>& IfStmt::getConditional() const
    {
        return conditional;
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

    ElseStmt::ElseStmt()
    : body {}
    {}

    const BlockStmt& ElseStmt::getBody() const
    {
        return body;
    }

    void ElseStmt::addStmt(std::unique_ptr<IStmt> stmt)
    {
        body.addStmt(std::move(stmt));
    }

    std::any ElseStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitElseStmt(*this);
    }

    /* WhileStmt impl. */

    WhileStmt::WhileStmt(std::unique_ptr<IExpr> conditional_expr)
    : conditional(std::move(conditional_expr)), body {}
    {}

    const std::unique_ptr<IExpr>& WhileStmt::getConditional() const
    {
        return conditional;
    }

    const BlockStmt& WhileStmt::getBody() const
    {
        return body;
    }

    void WhileStmt::addStmt(std::unique_ptr<IStmt> stmt)
    {
        body.addStmt(std::move(stmt));
    }

    std::any WhileStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitWhileStmt(*this);
    }

    /* BlockStmt impl. */

    BlockStmt::BlockStmt()
    : body {}
    {}

    const std::vector<std::unique_ptr<IStmt>>& BlockStmt::getBody() const
    {
        return body;
    }

    void BlockStmt::addStmt(std::unique_ptr<IStmt> stmt)
    {
        body.emplace_back(std::move(stmt));
    }

    std::any BlockStmt::accept(StmtVisitor<std::any>& visitor)
    {
        return visitor.visitBlockStmt(*this);
    }
}
