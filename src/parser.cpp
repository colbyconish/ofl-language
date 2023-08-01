#include "parser.hpp"

namespace ofl
{
    static std::set<std::string> KEYWORDS =
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
                    else if(charIs(CharType::Space, c))
                        if(buffer.size() > 0)
                        {
                            list.push_back({TokenType::Identifier, buffer});
                            buffer.clear();
                        }
                        else
                            continue;
                    else
                        if(buffer.size() > 0)
                        {
                            list.push_back({TokenType::Identifier, buffer});
                            list.push_back({tokenType(c), c}); 
                            buffer.clear();
                        }
                        else
                            list.push_back({tokenType(c), c}); 
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
                    else if(charIs(CharType::Space, c))
                        if(buffer.size() > 0)
                        {
                            list.push_back({TokenType::Literal, buffer});
                            buffer.clear();
                        }
                        else
                            continue;
                    else
                        if(buffer.size() > 0)
                        {
                            list.push_back({TokenType::Literal, buffer});
                            list.push_back({tokenType(c), c}); 
                            buffer.clear();
                        }
                        else
                            list.push_back({tokenType(c), c}); 
                    break;
            }
        }

        return true;
    }
}