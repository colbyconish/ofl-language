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

    void Executor::setValue(TypeInfo *type, void *ptr, const char *value)
    {
        if(type->name == "int")
            *((int*)ptr) = atoi(value);
        else if(type->name == "float")
            *((float*)ptr) = (float) atof(value);
        else
            throw executor_exception(MSG("Undefined type name: " + type->name.c_str()));
    }

    bool Executor::Execute(Node* root)
    {
        for(auto& node : *root->_children)
        {
            switch(node._type)
            {
                case NodeType::Declaration:
                {
                    TypeInfo *type = (TypeInfo *) (*node._children)[0]._data;
                    std::string *name = (std::string *) (*node._children)[1]._data;
                    std::string *value = (std::string *) (*node._children)[2]._data;
                    
                    // Check if variable already exists
                    auto it = variables.find(*name);
                    if(it != variables.end())
                        throw executor_exception(MSG("Variable already declared:" + *name));

                    // Allocate memory for declaration
                    void *ptr = _storage.Allocate(type->size);
                    if(ptr == nullptr)
                        throw executor_exception(MSG("Memory allocation error."));

                    // Assign value to data
                    setValue(type, ptr, value->c_str());

                    // Save variable
                    auto new_it = variables.emplace(*name, Variable{type, ptr});
                    break;
                }
                case NodeType::Invocation:
                {
                    std::string *name = (std::string *) (*node._children)[0]._data;
                    std::string *value = (std::string *) (*node._children)[1]._data;

                    // Chack for variable
                    auto it = variables.find(*value);
                    if(it == variables.end())
                        throw executor_exception(MSG("Undefined identifier: " + *value));

                    // Check for language functions
                    if(*name == "print")
                    {
                        if(it->second.type->name == "int")
                            printf("%d\n", *(int*)it->second.value);
                        else if(it->second.type->name == "float")
                            printf("%f\n", *(float*)it->second.value);
                        else
                            throw executor_exception(MSG("Cannont print type: " + it->second.type->name));
                        break;
                    }

                    // Check for user defined functions

                    throw executor_exception(MSG("Undefined function: " + *name));
                    break;
                }
                default:
                    throw executor_exception(MSG("Unrecognized node type: " + (int)node._type));
            }
        }

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
        if(start == nullptr) return;

        free(start);
        start = nullptr;
    }

    void *Memory::Allocate(size_t size)
    {
        if(start == nullptr) return nullptr;
        if((size_t) start + size >= (size_t)end) return nullptr;

        void* temp = start;
        *((size_t*)&start) += size;
        return temp;
    }
}