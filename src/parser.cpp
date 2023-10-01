#include "parser.hpp"

namespace ofl
{
    std::set<std::string> Parser::KEYWORDS =
    {
        "print",
        "exit",
        "when",
        "set",
        "if",
        "for",
        "while",
        "break",
        "continue",
        "using",
        "define",
        "invent"
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

        static std::string line;
        if(!std::getline(_stream, line)) return false;
        line += "\n";

        int pos = 0;
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
                        if(buffer.size() > 0)
                            PushIdentifier(list, buffer);
                        _mode = ReadMode::Operator;
                        buffer += c;
                    }
                    else if(charIs(CharType::Space, c) || charIs(CharType::NewLine, c))
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

                    if(pos == line.size()-1)
                    {
                        if(buffer.size() > 0)
                            PushIdentifier(list, buffer);
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
                        if(buffer.size() > 0)
                            PushLiteral(list, buffer);
                        _mode = ReadMode::Operator;
                        buffer += c;
                    }
                    else if(charIs(CharType::Space, c) || charIs(CharType::NewLine, c))
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

                    if(pos == line.size()-1)
                    {
                        if(buffer.size() > 0)
                            PushLiteral(list, buffer);
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
                        continue;
                    }
                    else if(charIs(CharType::Space, c) || charIs(CharType::NewLine, c))
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

                    if(pos == line.size()-1)
                    {
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                    }
                    break;
            }

            pos++;
        }

        if(_stream.peek() == EOF) list.push_back(Token::EndOfFile());
        return true;
    }

    void Parser::PushIdentifier(TokenList& list, std::string& buffer)
    {   
        auto it = KEYWORDS.find(buffer);
        if(it == KEYWORDS.end())
            list.push_back(Token::Identifier(buffer));
        else
            list.push_back(Token::Identifier(buffer, true));
        buffer.clear();
    }

    void Parser::PushLiteral(TokenList& list, std::string& buffer)
    {   
        list.push_back(Token::Literal(buffer));
        buffer.clear();
    }

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

    void Parser::PushOperator(TokenList& list, std::string& buffer)
    {   
        Op op = GetOperator(buffer);
        list.push_back(Token::Operator(op));
        buffer.clear();
    }
}