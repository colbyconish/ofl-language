#pragma once

#include "ast.hpp"
#include "token.hpp"

namespace ofl
{
    class Lexer
    {
    public:
        static Node* Lex(TokenList&);

    private:
    };
}