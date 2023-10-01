#pragma once

namespace ofl
{
    enum class CharType
    {
        Unknown,
        Letter,
        Upper, 
        Lower,
        Digit,
        Space,
        Paren,
        LeftParen,
        RightParen,
        Square,
        LeftSquare,
        RightSquare,
        Curly,
        LeftCurly,
        RightCurly,
        Operator,
        Equals,
        Delimeter,
        Comma,
        SemiColon,
        Colon,
        Quote,
        SingleQuote,
        DoubleQuote,
        NewLine
    };

    class Character
    {
    public:
        static const char A = 65;
        static const char a = 97;
        static const char Z = 90;
        static const char z = 122;
        static const char ZERO = 48;
        static const char NINE = 57;

        static const char SPACE = 32;
        static const char NEWLINE = 10;

        static const char SINGLEQUOTE = 39;
        static const char DOUBLEQUOTE = 34;

        static const char LEFTPAREN = 40;
        static const char RIGHTPAREN = 41;
        static const char LEFTSQUARE = 91;
        static const char RIGHTSQUARE = 93;
        static const char LEFTCURLY = 123;
        static const char RIGHTCURLY = 125;

        static const char SEMICOLON = 59;
        static const char COMMA = 44;
        static const char COLON = 58;

        static const char EQUALS = 61;
        static const char PLUS = 43;
        static const char MINUS = 45;
        static const char DIVIDE = 47;
        static const char MULT = 42;
        static const char LSHIFT = 60;
        static const char RSHIFT = 62;

    };

    inline bool charIs(CharType type, char c)
        {
            switch(type)
            {
                case CharType::Letter:
                    return (c >= Character::A && c <= Character::Z) || (c >= Character::a && c <= Character::z);
                    break;
                case CharType::Upper:
                    return c >= Character::A && c <= Character::Z;
                    break;
                case CharType::Lower:
                    return c >= Character::a && c <= Character::z;
                    break;
                case CharType::Space:
                    return c == Character::SPACE;
                    break;
                case CharType::Delimeter:
                    return c == Character::COMMA || c == Character::SEMICOLON;
                    break;
                case CharType::Paren:
                    return c == Character::LEFTPAREN || c == Character::RIGHTPAREN;
                    break;
                case CharType::LeftParen:
                    return c == Character::LEFTPAREN;
                    break;
                case CharType::RightParen:
                    return c == Character::RIGHTPAREN;
                    break;
                case CharType::Curly:
                    return c == Character::LEFTCURLY || c == Character::RIGHTCURLY;
                    break;
                case CharType::LeftCurly:
                    return c == Character::LEFTCURLY;
                    break;
                case CharType::RightCurly:
                    return c == Character::RIGHTCURLY;
                    break;
                case CharType::Square:
                    return c == Character::LEFTSQUARE || c == Character::RIGHTSQUARE;
                    break;
                case CharType::LeftSquare:
                    return c == Character::LEFTSQUARE;
                    break;
                case CharType::RightSquare:
                    return c == Character::RIGHTSQUARE;
                    break;
                case CharType::Digit:
                    return c >= Character::ZERO && c <= Character::NINE;
                    break;
                case CharType::Operator:
                    return c == Character::EQUALS || c == Character::COLON ||
                    c == Character::PLUS || c == Character::MINUS ||
                    c == Character::MULT || c == Character::DIVIDE ||
                    c == Character::RSHIFT || c == Character::LSHIFT; // ^ %
                    break;
                case CharType::Equals:
                    return c == Character::EQUALS;
                    break;
                case CharType::NewLine:
                    return c == Character::NEWLINE;
                    break;
                case CharType::Quote:
                    return c == Character::SINGLEQUOTE || c == Character::DOUBLEQUOTE;
                    break;
                case CharType::SingleQuote:
                    return c == Character::SINGLEQUOTE;
                    break;
                case CharType::DoubleQuote:
                    return c == Character::DOUBLEQUOTE;
                    break;
            }

            return false;
        }

        //rewrite this using a map
        inline CharType charIs(char c)
        {
            if(c >= Character::A && c <= Character::Z)
                return CharType::Upper;
            else if(c >= Character::a && c <= Character::z)
                return CharType::Lower;
            else if(c >= Character::ZERO && c <= Character::NINE)
                return CharType::Digit;
            else if(c == Character::SPACE)
                return CharType::Space;
            else if(c == Character::NEWLINE)
                return CharType::Delimeter;
            else if(c == Character::LEFTPAREN)
                return CharType::LeftParen;
            else if(c == Character::RIGHTPAREN)
                return CharType::RightParen;
            else if(c == Character::LEFTSQUARE)
                return CharType::LeftSquare;
            else if(c == Character::RIGHTSQUARE)
                return CharType::RightSquare;
            else if(c == Character::LEFTCURLY)
                return CharType::LeftCurly;
            else if(c == Character::RIGHTCURLY)
                return CharType::RightCurly;
            else if(c == Character::EQUALS)
                return CharType::Equals;
            else if(c == Character::COLON)
                return CharType::Colon;
            else if(c == Character::COMMA)
                return CharType::Comma;
            else if(c == Character::SEMICOLON)
                return CharType::SemiColon;
            else if(c == Character::SINGLEQUOTE)
                return CharType::SingleQuote;
            else if(c == Character::DOUBLEQUOTE)
                return CharType::DoubleQuote;
            
            return CharType::Unknown;
        }
}