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
        static void setValue(TypeMap::iterator &type, VariationMap::iterator &variation, void *ptr, const char *value);
        Executor();
        ~Executor();

        bool Execute(Node* root);

        std::map<std::string, Variable> variables;
        TypeMap types = 
        {
            {"int", 
                {
                    "int",
                    "32",
                    {
                        {"32", 4},
                        {"64", 8}
                    },
                    {TokenType::Literal}
                }
            },
            {"dec", 
                {
                    "dec",
                    "32",
                    {
                        {"32", 4},
                        {"64", 8}
                    },
                    {TokenType::Literal}
                }
            }
        };

    private:
        Memory _storage = Memory();
    };
}