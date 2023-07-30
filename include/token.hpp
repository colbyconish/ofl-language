#pragma once

#include <vector>
#include <string>

namespace ofl
{
    struct Token;
    typedef std::vector<Token> TokenList;

    enum class TokenType
    {
        Brace,
        Operator,
        Punctuation,
        Keyword,
        Identifier,
        Literal
    };

    struct Token
    {
        TokenType _type;
        std::string _contents;
    };
}