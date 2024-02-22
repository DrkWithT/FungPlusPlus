#ifndef STMTBASE_HPP
#define STMTBASE_HPP

#include <any>
#include "syntax/stmtvisitor.hpp"

namespace fung::syntax
{
    class IStmt
    {
    private:

    public:
        virtual ~IStmt() = default;

        virtual std::any accept(StmtVisitor<std::any>& visitor) = 0;
    };
}

#endif