#include "parser.hpp"

namespace ofl
{
    std::set<std::string> Parser::KEYWORDS =
    {
        "int",
        "float"
    };

    Parser::Parser(const char* infile)
    {
        if(infile != nullptr)
            Open(infile);
    }
    
    Parser::~Parser()
    {
        if(!_ready) return;

        Close();
    }

    bool Parser::Open(const char* path)
    {
        _stream.open(path);
        if(_stream.fail())
        {
            LOG(LogSeverity::Error, "Error opening file: " + std::strerror(errno));
            return false;
        }

        _ready = true;
        return true;
    }

    bool Parser::Close()
    {
        _ready = false;
        _stream.close();
        return true;
    }

    bool Parser::ParseFile(TokenList& list)
    {
        if(!_ready) return false;

        

        return true;
    }

    bool Parser::ParseLine(TokenList& list)
    {
        if(!_ready) return false;

        std::string line;
        if(!std::getline(_stream, line)) return false;


        std::string buffer;
        for(auto& c : line)
        {
            switch(_mode)
            {
                case ReadMode::Letter:
                    if(charIs(CharType::Letter, c))
                        buffer += c;
                    else if(charIs(CharType::Digit, c))
                    {
                        if(buffer.size() == 0)
                            _mode = ReadMode::Number;
                        buffer += c;
                    }
                    else if(charIs(CharType::Operator, c))
                    {
                        if(buffer.size() == 0)
                        {
                            _mode = ReadMode::Operator;
                        }
                        buffer += c;
                    }
                    else if(charIs(CharType::Space, c))
                        if(buffer.size() > 0)
                            PushIdentifier(list, buffer);
                        else
                            continue;
                    else
                    {
                        if(buffer.size() > 0)
                            PushIdentifier(list, buffer);
                        list.push_back(Token::FromCharacter(c)); 
                    }
                    break;
                case ReadMode::Number:
                    if(charIs(CharType::Digit, c))
                        buffer += c;
                    else if(charIs(CharType::Letter, c))
                    {
                        if(buffer.size() == 0)
                            _mode = ReadMode::Letter;
                        buffer += c;
                    }
                    else if(charIs(CharType::Operator, c))
                    {
                        if(buffer.size() == 0)
                            _mode = ReadMode::Operator;
                        buffer += c;
                    }
                    else if(charIs(CharType::Space, c))
                        if(buffer.size() > 0)
                            PushLiteral(list, buffer);
                        else
                            continue;
                    else
                    {
                        if(buffer.size() > 0)
                            PushLiteral(list, buffer);
                        list.push_back(Token::FromCharacter(c)); 
                    }
                    break;
                case ReadMode::Operator:
                    if(charIs(CharType::Operator, c))
                        buffer += c;
                    else if(charIs(CharType::Letter, c))
                    {
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        _mode = ReadMode::Letter;
                        buffer = c;
                    }
                    else if(charIs(CharType::Digit, c))
                    {
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        _mode = ReadMode::Number;
                        buffer = c;
                    }
                    else if(charIs(CharType::Space, c))
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        else
                            continue;
                    else
                    {
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        list.push_back(Token::FromCharacter(c)); 
                    } 
                    break;
            }
        }

        return true;
    }

    void Parser::PushIdentifier(TokenList& list, std::string& buffer)
    {   
        auto it = KEYWORDS.find(buffer);
        if(it == KEYWORDS.end())
            list.push_back(Token::Identifier(buffer));
        else
            list.push_back(Token::Identifier(buffer));
        buffer.clear();
    }

    void Parser::PushLiteral(TokenList& list, std::string& buffer)
    {   
        list.push_back(Token::Literal(buffer));
        buffer.clear();
    }

    void Parser::PushOperator(TokenList& list, std::string& buffer)
    {   
        Op op = GetOperator(buffer);
        list.push_back(Token::Operator(op));
        buffer.clear();
    }
}