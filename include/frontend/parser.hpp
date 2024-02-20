#ifndef PARSER_HPP
#define PARSER_HPP

#include "frontend/lexer.hpp"
#include "syntax/expressions.hpp"

// using FungCallExpr = fung::syntax::CallExpr;
// using FungElemExpr = fung::syntax::ElementExpr;
// using FungAcssExpr = fung::syntax::AccessExpr;
// using FungUnryExpr = fung::syntax::UnaryExpr;
// using FungBnryExpr = fung::syntax::BinaryExpr;

namespace fung::frontend
{
    enum ParserStatus
    {
        fung_parse_ok,
        fung_parse_unexpected_token,
        fung_parse_generic_error
    };

    struct ParserDumpState
    {
        Token error_token;
        ParserStatus status;
    };

    class ProgramUnit
    {
    private:
        std::vector<std::any> statements;
        std::string name;
    public:
        ProgramUnit(const std::string& unit_name);

        const std::vector<std::any>& getStatements() const;
        const std::string& getName() const;
    };

    class Parser
    {
    private:
        Lexer lexer;
        Token previous;
        Token current;
    public:
        Parser(const std::string_view& source_view);

        fung::syntax::ElementExpr parseElement();

        fung::syntax::CallExpr parseCall();

        fung::syntax::AccessExpr parseAccess();

        fung::syntax::UnaryExpr parseUnary();

        fung::syntax::BinaryExpr parseBinary();

        /// TODO Add AST nodes for stmts...
        /// TODO Add parsing methods for stmts...

        ParserDumpState parseFile(ProgramUnit& unit);
    };
}

#endif