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

    void Executor::setValue(TypeMap::iterator &type, VariationMap::iterator &variation, void *ptr, const char *value)
    {
        if(type->first == "int")
        {
            if(variation->second.size == 4)
                *((int32_t*)ptr) = atoi(value);
            else if(variation->second.size == 8)
                *((int64_t*)ptr) = _atoi64(value);
        }
        else if(type->first == "dec")
        {
            if(variation->second.size == 4)
                *((float*)ptr) = (float) atof(value);
            else if(variation->second.size == 8)
                *((double*)ptr) = atof(value);
        }
        else
            throw executor_exception(MSG("Undefined type name: " + type->first));
    }

    bool Executor::Execute(Node* root)
    {
        for(auto& node : *root->children)
        {
            switch(node.type)
            {
                case NodeType::Declaration:
                {
                    TypeInstance *type = (TypeInstance *) (*node.children)[0].data;
                    std::string *name = (std::string *) (*node.children)[1].data;
                    std::string *value = (std::string *) (*node.children)[2].data;
                    
                    // Check if variable already exists
                    auto it = variables.find(*name);
                    if(it != variables.end())
                        throw executor_exception(MSG("Variable already declared:" + *name));

                    auto type_it = types.find(type->type);
                    auto variation_it = type_it->second.GetVariation(type->variation);

                    // Allocate memory for declaration
                    void *ptr = _storage.Allocate(variation_it->second.size);
                    if(ptr == nullptr)
                        throw executor_exception(MSG("Memory allocation error."));

                    // Assign value to data
                    setValue(type_it, variation_it, ptr, value->c_str());

                    // Save variable
                    auto new_it = variables.emplace(*name, Variable{*type, ptr});
                    break;
                }
                case NodeType::Invocation:
                {
                    std::string *name = (std::string *) (*node.children)[0].data;
                    std::string *value = (std::string *) (*node.children)[1].data;

                    // Chack for variable
                    auto it = variables.find(*value);
                    if(it == variables.end())
                        throw executor_exception(MSG("Undefined identifier: " + *value));

                    // Check for language functions
                    if(*name == "print")
                    {
                        auto type = types.find(it->second.type.type);
                        auto variation = type->second.GetVariation(it->second.type.variation);
                        
                        if(type->first == "int")
                        {
                            if(variation->second.size == 4)
                                std::cout << *(int32_t*) it->second.value << std::endl;
                            else if(variation->second.size == 8)
                                std::cout << *(int64_t*) it->second.value << std::endl;
                        }
                        else if(type->first == "dec")
                        {
                            if(variation->second.size == 4)
                                std::cout << *(float*) it->second.value << std::endl;
                            else if(variation->second.size == 8)
                                std::cout << *(double*) it->second.value << std::endl;
                        }
                        else
                            throw executor_exception(MSG("Cannont print type: " + type->first));
                        break;
                    }

                    // Check for user defined functions

                    throw executor_exception(MSG("Undefined function: " + *name));
                    break;
                }
                default:
                    throw executor_exception(MSG("Unrecognized node type: " + (int)node.type));
            }
        }

        return true;
    }

    Memory::Memory(size_t initial_size)
    {
        data = malloc(initial_size);
        if(data == nullptr) return;

        start = data;
        size = initial_size;
        end = (void*) ((size_t) start+initial_size);
    }

    Memory::Memory(Memory&& other)
    {
        data = other.data;
        start = other.start;
        end = other.end;
        size = other.size;

        other.data = nullptr;
        other.start = nullptr;
    }

    Memory::~Memory()
    {
        if(data == nullptr) return;

        free(data);
        data = nullptr;
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