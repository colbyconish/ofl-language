#pragma once

#include <map>
#include <set>

#include "ast.hpp"
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

        Lexer();
        Lexer(Lexer&) = delete;
        Lexer(Lexer&&) = delete;
        
        Node Lex(TypeMap&);
        size_t checkForFunctionCall(const std::string&, TokenList&, Node*, size_t);

        TokenList _tokens;

    private:
    
        size_t _pos = 0;
    };
}