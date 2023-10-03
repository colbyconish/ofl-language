#pragma once

#include <map>
#include <set>

#include "ast.hpp"
#include "char.hpp"
#include "token.hpp"
#include "type.hpp"
#include "type.hpp"
#include "logger.hpp"
#include "exception.hpp"

namespace ofl
{
    struct Assignment
    {
        std::string name;
        Token* value;
    };

    class Lexer
    {
    public:
        static size_t checkForAssignment(TypeMap::iterator&, TokenList&, Node*, size_t);
        size_t checkForLoop(const std::string&, TypeMap&, TokenList&, Node*, size_t);
        static size_t checkForFunctionCall(const std::string&, TokenList&, Node*, size_t);
        size_t checkForSequence(Node*, TypeMap&, size_t, bool = false);

        Lexer();
        Lexer(Lexer&) = delete;
        Lexer(Lexer&&) = delete;
        
        Node Lex(TypeMap&);

        TokenList _tokens;
    };
}