#pragma once

#include <vector>
#include <string>

#include "char.hpp"

namespace ofl
{
    struct Token;
    typedef std::vector<Token> TokenList;

    enum class TokenType
    {
        Unknown,
        Square,
        Curly,
        Paren,
        Operator,
        Keyword,
        Identifier,
        Literal,
        Delemiter,
        ENDOFFILE
    };

    inline TokenType tokenType(char c)
    {
        if(charIs(CharType::Paren, c))
            return TokenType::Paren;
        else if(charIs(CharType::Square, c))
            return TokenType::Square;
        else if(charIs(CharType::Curly, c))
            return TokenType::Curly;
        else if(charIs(CharType::Delimeter, c))
            return TokenType::Delemiter;
        else
            return TokenType::Unknown;
    }

    struct Token
    {
        TokenType _type;
        std::string _contents;
        Token(TokenType type, std::string& str)
            :_type(type), _contents(str) { }
        Token(TokenType type, char str)
            :_type(type), _contents("") 
            { _contents += str; }
        ~Token() { }
    };
}