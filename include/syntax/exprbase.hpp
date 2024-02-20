#ifndef EXPRBASE_HPP
#define EXPRBASE_HPP

#include <any>
#include "syntax/exprvisitor.hpp"

namespace fung::syntax
{
    class IExpr
    {
    public:
        virtual ~IExpr() = default;

        virtual std::any accept(ExprVisitor<std::any>& visitor) = 0;
    };
}

#endif