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
        void* Allocate(size_t size);
    };

    struct Variable
    {
        TypeInstance type;
        void *value;
    };

    class Executor
    {
    public:
        Executor();
        ~Executor();

        bool Execute(Node* root);

        std::map<std::string, Variable> variables;
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
                    &print_string
                }
            }
        };

    private:
        Memory _storage = Memory();
    };
}