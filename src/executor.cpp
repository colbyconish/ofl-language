#include "executor.hpp"
#include "parser.hpp"

namespace ofl
{
    Executor::Executor()
    {

    }

    Executor::~Executor()
    {

    }

    bool Executor::Execute(Node* root)
    {
        
        return true;
    }

    Memory::Memory(size_t initial_size)
    {
        start = malloc(initial_size);
        if(start == nullptr) return;

        size = initial_size;
        end = (void*) ((size_t) start+initial_size);
    }

    Memory::Memory(Memory&& other)
    {
        start = other.start;
        end = other.end;
        size = other.size;

        other.start = nullptr;
    }

    Memory::~Memory()
    {
        if(start != nullptr) return;

        free(start);
        start = nullptr;
    }

    void* Memory::Allocate(size_t size)
    {
        if(start == nullptr) return nullptr;
        if((size_t) start + size >= (size_t)end) return nullptr;

        void* temp = start;
        *((size_t*)&start) += size;
        return temp;
    }
}