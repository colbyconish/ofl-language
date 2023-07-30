#pragma once

#include <fstream>
#include <cstring>

#include "token.hpp"
#include "logger.hpp"

namespace ofl
{
    class Parser
    {
    public:
        Parser(const char* infile = nullptr);
        Parser(Parser&) = delete;
        Parser(Parser&&) = delete;
        ~Parser();

        bool Open(const char* path);
        bool Close();

        bool ParseFile(TokenList& list);
        bool ParseLine(TokenList& list); 

    private:
        std::ifstream _stream;

        size_t _pos;
        bool _ready;
    };
}