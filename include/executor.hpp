#pragma once
#include <map>
#include <string>
#include <iostream>

#include "type.hpp"
#include "ast.hpp"

#define INITIAL_SIZE 1024

namespace ofl
{
    struct Memory
    {
        void *data = nullptr;
        void *start = nullptr;
        void *end = nullptr;
        size_t size = 0;

        //Memory() = delete;
        Memory(size_t initial_size = INITIAL_SIZE);
        Memory(Memory&) = delete;
        Memory(Memory&&);
        ~Memory();
        void *Allocate(size_t size);
        void Deallocate(size_t size);
    };

    struct Variable
    {
        TypeInstance type;
        void *value;
    };

    typedef std::set<std::string> VariableList;
    typedef std::map<std::string, Variable > VariableMap;
    typedef std::vector<VariableList > ScopeList;

    class Executor
    {
    public:
        Executor();
        ~Executor();

        bool Execute(Node* root, bool original = true);

        TypeMap types = 
        {
            {
                "int",
                {
                    "integer",
                    "32",
                    {
                        {"32", sizeof(int32_t)},
                        {"64", sizeof(int64_t)}
                    },
                    {TokenType::NumberLiteral},
                    &assign_integer,
                    &destruct_default,
                    &print_integer
                }
            },
            {
                "dec",
                {
                    "decemial",
                    "32",
                    {
                        {"32", sizeof(float)},
                        {"64", sizeof(double)}
                    },
                    {TokenType::NumberLiteral},
                    &assign_decimal,
                    &destruct_default,
                    &print_decimal
                }
            },
            {
                "bin",
                {
                    "binary",
                    "default",
                    {
                        {"default", sizeof(int8_t)}
                    },
                    {TokenType::Keyword, TokenType::NumberLiteral},
                    &assign_boolean,
                    &destruct_default,
                    &print_boolean
                }
            },
            {
                "str",
                {
                    "string",
                    "default",
                    {
                        {"default", sizeof(std::string*)},
                    },
                    {TokenType::StringLiteral},
                    &assign_string,
                    &destruct_string,
                    &print_string
                }
            }
        };

    private:
        ScopeList _scopes;
        VariableMap _variables;
        Memory _storage = Memory();
    };
}