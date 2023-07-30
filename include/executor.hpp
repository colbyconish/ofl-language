#pragma once

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

    class Executor
    {
    public:
        Executor();
        ~Executor();

        bool Execute(Node* root);

    private:
        Memory _storage = Memory();
    };
}