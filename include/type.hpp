#pragma once

#include <string>
#include <set>

#include "token.hpp"

namespace ofl
{
    struct TypeInfo
    {
        std::string name;
        size_t size;
        TypeInfo(const char* name, size_t size, std::set<TokenType> set)
            :name(name), size(size), _accepts(set) { }
        bool Accepts(TokenType type)
        {
            return _accepts.find(type) != _accepts.end();
        }
    private:
        std::set<TokenType> _accepts;
        TypeInfo();
    };
}