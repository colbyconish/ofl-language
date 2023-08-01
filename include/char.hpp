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
        COMMA,
        SEMICOLON
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

        static const char LEFTPAREN = 40;
        static const char RIGHTPAREN = 41;
        static const char LEFTSQUARE = 91;
        static const char RIGHTSQUARE = 93;
        static const char LEFTCURLY = 123;
        static const char RIGHTCURLY = 125;

        static const char SEMICOLON = 59;
        static const char COMMA = 44;



        static const char EQUALS = 61;
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
                    return c == Character::EQUALS; //add * - / + ^ %
                case CharType::Equals:
                    return c == Character::EQUALS;
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
            else if(c == Character::COMMA)
                return CharType::COMMA;
            else if(c == Character::SEMICOLON)
                return CharType::SEMICOLON;
            
            return CharType::Unknown;
        }
}