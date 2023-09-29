#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "char.hpp"
#include "exception.hpp"

namespace ofl
{
    typedef uint64_t Op;

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

    inline const char* to_string(TokenType type)
    {
        const char* names[] = 
        {
            "Unknown",
            "Square",
            "Curly",
            "Paren",
            "Operator",
            "Keyword",
            "Identifier",
            "Literal",
            "Delemiter",
            "ENDOFFILE"
        };

        return names[(int) type];
    }

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
    public:
        Token(Token&) = delete;
        Token(Token&& other)
            :type(other.type), data(other.data)
        {
            other.type = TokenType::Unknown;
            other.data = nullptr;
        }
        static Token Literal(std::string& str)
        {
            Token t;
            t.type = TokenType::Literal;
            t.data = new std::string(str);

            return std::move(t);
        }
        static Token Identifier(std::string& str)
        {
            Token t;
            t.type = TokenType::Identifier;
            t.data = new std::string(str);

            return std::move(t);
        }
        static Token Operator(Op op)
        {
            Token t;
            t.type = TokenType::Operator;
            *((Op*) &t.data) = op;

            return std::move(t);
        }
        static Token FromCharacter(char c)
        {
            Token t;
            t.type = tokenType(c);
            *((char*) &t.data) = c;
            return std::move(t);
        }
        static Token EndOfFile()
        {
            Token t;
            t.type = TokenType::ENDOFFILE;
            return std::move(t);
        }

        ~Token()
        {
            if(type == TokenType::Unknown) return;

            if((type == TokenType::Identifier || type == TokenType::Literal) && data != nullptr)
                delete (std::string*) data;
        }

        std::string to_string()
        {
            std::string temp;
            temp += std::to_string((int) type); 
            temp += ':';
            temp += '[';

            if((type == TokenType::Identifier || type == TokenType::Literal) && data != nullptr)
                temp += *((std::string*) data);
            else if(type == TokenType::Operator)
                temp += ((const char*) &data);
            else if(type == TokenType::ENDOFFILE)
                temp += "EOF";
            else
                temp += *((int*) &data);
            temp += ']';
            return temp;
        }

        inline TokenType Type() {return type;}
        inline void* Data() {return data;}
private:
    Token() :type(TokenType::Unknown), data(nullptr){ }

    TokenType type = TokenType::Unknown;
    void* data = nullptr;

    size_t line, col;
    };

    inline Op GetOperator(std::string& buffer)
    {
        if(buffer.size() > 3)
        {
            std::string message = "Operator too long: " + buffer;
            throw parser_exception(message);
        }

        Op op = 0;
        for(int i = buffer.size()-1;i >= 0;i--)
        {
            op <<= sizeof(char)*8;
            op = op | buffer[i];
        }

        return op;
    }
}