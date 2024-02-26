#ifndef STMTVISITOR_HPP
#define STMTVISITOR_HPP

namespace fung::syntax
{
    /* Forward declare visitable classes to resolve circular referencing in visitor pattern... */

    class UseStmt;
    class VarStmt;
    class ParamDecl;
    class FuncDecl;
    class FieldDecl;
    class ObjectDecl;
    class AssignStmt;
    class ReturnStmt;
    class IfStmt;
    class ElseStmt;
    class WhileStmt;
    class BlockStmt;
    class ExprStmt;

    /**
     * @brief Interface for AST walking components e.g a naive code generator.
     * 
     * @tparam ResultType 
     */
    template<typename ResultType>
    class StmtVisitor
    {
    public:
        virtual ~StmtVisitor() = default;

        virtual ResultType visitUseStmt(const UseStmt& stmt) = 0;
        virtual ResultType visitVarStmt(const VarStmt& stmt) = 0;
        virtual ResultType visitParamDecl(const ParamDecl& stmt) = 0;
        virtual ResultType visitFuncDecl(const FuncDecl& stmt) = 0;
        virtual ResultType visitFieldDecl(const FieldDecl& stmt) = 0;
        virtual ResultType visitObjectDecl(const ObjectDecl& stmt) = 0;
        virtual ResultType visitAssignStmt(const AssignStmt& stmt) = 0;
        virtual ResultType visitReturnStmt(const ReturnStmt& stmt) = 0;
        virtual ResultType visitIfStmt(const IfStmt& stmt) = 0;
        virtual ResultType visitElseStmt(const ElseStmt& stmt) = 0;
        virtual ResultType visitWhileStmt(const WhileStmt& stmt) = 0;
        virtual ResultType visitExprStmt(const ExprStmt& stmt) = 0;
        virtual ResultType visitBlockStmt(const BlockStmt& stmt) = 0;
    };
}

#endif