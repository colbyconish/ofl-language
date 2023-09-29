#pragma once

#include <map>
#include <set>

#include "ast.hpp"
#include "token.hpp"
#include "type.hpp"
#include "type.hpp"
#include "logger.hpp"

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
        Lexer();
        Lexer(Lexer&) = delete;
        Lexer(Lexer&&) = delete;
        
        Node Lex(std::map<std::string, TypeInfo>&);
        size_t checkForAssignment(TypeInfo&, TokenList&, Node*, size_t);
        size_t checkForFunctionCall(const std::string&, TokenList&, Node*, size_t);

        TokenList _tokens;

        static const Op EQUALS_OP = 0x3D; 

        static std::set<std::string> keywords;
    private:
    
        size_t _pos = 0;
    };
}