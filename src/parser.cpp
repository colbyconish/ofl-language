#include "parser.hpp"

namespace ofl
{
    std::set<std::string> Parser::KEYWORDS =
    {
        "print",
        "exit",
        "when",
        "set",
        "unset",
        "if",
        "for",
        "while",
        "break",
        "continue",
        "using",
        "define",
        "invent",
        "true",
        "false",
        "yes",
        "no",
        "is",
        "and",
        "or",
        "xor",
        "void"
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

    bool Parser::Scoped()
    {
        return _scopes > 0;
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
        static std::string buffer;
        for(auto& c : line)
        {
            if(_mode !=  ReadMode::String && charIs(CharType::Curly, c))
                _scopes += (charIs(CharType::LeftCurly, c) ? 1 : -1);

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
                    else if(charIs(CharType::DoubleQuote, c))
                    {
                        _mode = ReadMode::String;
                        if(buffer.size() > 0)
                            PushIdentifier(list, buffer);
                        else
                            continue;
                    }
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
                    else if(charIs(CharType::DoubleQuote, c))
                    {
                        _mode = ReadMode::String;
                        if(buffer.size() > 0)
                            PushLiteral(list, buffer);
                        else
                            continue;
                    }
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
                        continue;
                    }
                    else if(charIs(CharType::Space, c) || charIs(CharType::NewLine, c))
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        else
                            continue;
                    else if(charIs(CharType::DoubleQuote, c))
                    {
                        _mode = ReadMode::String;
                        if(buffer.size() > 0)
                            PushOperator(list, buffer);
                        else
                            continue;
                    }
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
                case ReadMode::String:
                    if(charIs(CharType::DoubleQuote, c))
                    {
                        if(buffer.size() > 0)
                            PushString(list, buffer);
                        _mode = ReadMode::Letter;
                    }
                    else if(charIs(CharType::NewLine, c))
                        continue;
                    else
                       buffer += c;
            }

            pos++;
        }

        if(_stream.peek() == EOF) list.push_back(Token::EndOfFile());
        return true;
    }

    void Parser::PushString(TokenList& list, std::string& buffer)
    {
        list.push_back(Token::String(buffer));
        buffer.clear();
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
        list.push_back(Token::Number(buffer));
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