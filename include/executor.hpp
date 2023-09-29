#pragma once
#include <map>

#include "type.hpp"
#include "ast.hpp"

#define INITIAL_SIZE 1024

namespace ofl
{
    struct Memory
    {
        void* start = nullptr;
        void* end = nullptr;
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
        TypeInfo *type;
        void *value;
    };

    class Executor
    {
    public:
        static void setValue(TypeInfo *type, void *ptr, const char *value);
        Executor();
        ~Executor();

        bool Execute(Node* root);

        std::map<std::string, Variable> variables;
        std::map<std::string, TypeInfo> types = 
        {
            {"int", {"int", 4, {TokenType::Literal}}},
            {"float", {"float", 4, {TokenType::Literal}}}
        };

    private:
        Memory _storage = Memory();
    };
}