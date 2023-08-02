#pragma once

#include <fstream>
#include <cstring>
#include <set>

#include "char.hpp"
#include "token.hpp"
#include "logger.hpp"

enum class ReadMode
{
    Letter,
    Number,
    Operator
};

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

        void PushIdentifier(TokenList& list, std::string& buffer);
        void PushLiteral(TokenList& list, std::string& buffer);
        void PushOperator(TokenList& list, std::string& buffer);

    private:
        static std::set<std::string> KEYWORDS;

        ReadMode _mode = ReadMode::Letter;
        std::ifstream _stream;

        size_t _pos;
        bool _ready;
    };
}