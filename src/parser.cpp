#include "parser.hpp"

namespace ofl
{
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
            LOG(LogSeverity::Error, "Error opening file: " << std::strerror(errno));
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

        return true;
    }
}