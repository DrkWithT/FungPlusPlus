#ifndef EXPRVISITOR_HPP
#define EXPRVISITOR_HPP

namespace fung::syntax
{
    /* Forward declare visitable classes to resolve circular referencing in visitor pattern... */
 
    class CallExpr;
    class ElementExpr; // Contains primitive-type | idenfitier | call-expr
    class AccessExpr;
    class UnaryExpr;
    class BinaryExpr;

    /**
     * @brief Interface for AST walking components e.g a naive bytecode generator.
     * 
     * @tparam ResultType Usually a ByteCodeChunk (planned).
     */
    template<typename ResultType>
    class ExprVisitor
    {
    public:
        ~ExprVisitor() = default;

        virtual ResultType visitCallExpr(const CallExpr& expr) = 0;
        virtual ResultType visitElementExpr(const ElementExpr& expr) = 0;
        virtual ResultType visitAccessExpr(const AccessExpr& expr) = 0;
        virtual ResultType visitUnaryExpr(const UnaryExpr& expr) = 0;
        virtual ResultType visitBinaryExpr(const BinaryExpr& expr) = 0;
    };
}

#endif