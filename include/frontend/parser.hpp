#ifndef PARSER_HPP
#define PARSER_HPP

#include <initializer_list>
#include "frontend/lexer.hpp"
#include "syntax/expressions.hpp"
#include "syntax/statements.hpp"

namespace fung::frontend
{
    class ProgramUnit
    {
    private:
        std::vector<std::unique_ptr<fung::syntax::IStmt>> statements;
        std::string name;
    public:
        ProgramUnit(const std::string& unit_name);

        const std::vector<std::unique_ptr<fung::syntax::IStmt>>& getStatements() const;
        const std::string& getName() const;
    };

    class Parser
    {
    private:
        Lexer lexer;
        std::string_view source_viewer;
        Token previous;
        Token current;

        /* Token helpers */

        const Token& getPrevious() const;
        const Token& getCurrent() const;

        [[nodiscard]] bool matchToken(TokenType type);
        [[nodiscard]] Token advanceToken();

        void consumeToken(std::initializer_list<TokenType> types);

        void reportError(const Token& token, const std::string& msg_header);
        void synchronizeParse();

        /* Expr helpers */

        std::vector<std::unique_ptr<fung::syntax::IExpr>> parseListLiteral();
        std::vector<std::unique_ptr<fung::syntax::IExpr>>  parseObjectLiteral();
        std::unique_ptr<fung::syntax::IExpr> parseElement();
        std::unique_ptr<fung::syntax::IExpr> parseCall();
        std::unique_ptr<fung::syntax::IExpr> parseAccess();
        std::unique_ptr<fung::syntax::IExpr> parseUnary();
        std::unique_ptr<fung::syntax::IExpr> parseFactor();
        std::unique_ptr<fung::syntax::IExpr> parseTerm();
        std::unique_ptr<fung::syntax::IExpr> parseComparison();
        std::unique_ptr<fung::syntax::IExpr> parseConditional();

        /* Stmt helpers */

        std::unique_ptr<fung::syntax::IStmt> parseUse();
        std::unique_ptr<fung::syntax::IStmt> parseVar();
        std::unique_ptr<fung::syntax::IStmt> parseParam();
        std::unique_ptr<fung::syntax::IStmt> parseFunc();
        std::unique_ptr<fung::syntax::IStmt> parseField();
        std::unique_ptr<fung::syntax::IStmt> parseObject();
        std::unique_ptr<fung::syntax::IStmt> parseAssignOrExprStmt();
        std::unique_ptr<fung::syntax::IStmt> parseReturn();
        std::unique_ptr<fung::syntax::IStmt> parseIf();
        std::unique_ptr<fung::syntax::IStmt> parseElse();
        std::unique_ptr<fung::syntax::IStmt> parseWhile();
        std::unique_ptr<fung::syntax::IStmt> parseBlock();
        std::unique_ptr<fung::syntax::IStmt> parseSubStmt();
        std::unique_ptr<fung::syntax::IStmt> parseStmt();
    public:
        Parser(const char* source_cptr, size_t source_size);

        [[nodiscard]] bool parseFile(ProgramUnit& unit);
    };
}

#endif