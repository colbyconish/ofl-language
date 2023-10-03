#include "executor.hpp"
#include "parser.hpp"

namespace ofl
{
    Executor::Executor()
    {
        
    }

    Executor::~Executor()
    {
        // Destroy all globally scoped variables
        for(auto& var : _variables)
        {
            auto type_it = types.find(var.second.type.type);
            auto variation_it = type_it->second.GetVariation(var.second.type.variation); 

            //call destructor
            type_it->second.destruct(type_it, variation_it, var.second.value);
        }
    }

    bool Executor::Execute(Node* root, bool original)
    {
        VariableList scope = VariableList();

        for(auto& node : *root->children)
        {
            switch(node.type)
            {
                case NodeType::Sequence:
                    Execute(&node, false);
                    break;
                case NodeType::Repeat:
                {
                    std::string *mult = (std::string *) (*node.children)[0].data;
                    Node *body = &(*node.children)[1];

                    for(uint64_t i = 0;i < _atoi64(mult->c_str());i++)
                        Execute(body, false);
                    break;
                }
                case NodeType::Declaration:
                {
                    TypeInstance *type = (TypeInstance *) (*node.children)[0].data;
                    std::string *name = (std::string *) (*node.children)[1].data;
                    std::string *value = (std::string *) (*node.children)[2].data;
                    
                    // Check if variable already exists
                    auto it = _variables.find(*name);
                    if(it != _variables.end())
                        throw executor_exception(MSG("Variable already declared: " + *name));

                    auto type_it = types.find(type->type);
                    auto variation_it = type_it->second.GetVariation(type->variation);

                    // Allocate memory for declaration
                    void *ptr = _storage.Allocate(variation_it->second.size);
                    if(ptr == nullptr)
                        throw executor_exception(MSG("Memory allocation error."));

                    // Assign value to data
                    (*type_it->second.assign)(type_it, variation_it, ptr, (void *) value->c_str());

                    // Save variable and add to scope
                    _variables.emplace(*name, Variable{*type, ptr});
                    scope.insert(*name);
                    break;
                }
                case NodeType::Invocation:
                {
                    std::string *name = (std::string *) (*node.children)[0].data;
                    std::string *value = (std::string *) (*node.children)[1].data;

                    auto it = _variables.find(*value);

                    // Check for print function
                    if(*name == "print")
                    {
                        // Check if value is a nil value
                        if(it == _variables.end())
                        {
                            auto nil_it = types.find("nil");
                            auto var_it = nil_it->second.DefaultVariation();

                            // Calls nil's print function
                            (*nil_it->second.print)(nil_it, var_it, nullptr);
                        } 
                        else
                        {
                            auto type_it = types.find(it->second.type.type);
                            auto variation_it = type_it->second.GetVariation(it->second.type.variation);
                            
                            // Call type's print function
                            (*type_it->second.print)(type_it, variation_it, it->second.value);
                        }

                        break;
                    }

                    // Chack for variable
                    if(it == _variables.end())
                        throw executor_exception(MSG("Undefined identifier: " + *value));

                    // Check for user defined functions

                    throw executor_exception(MSG("Undefined function: " + *name));
                    break;
                }
                default:
                    throw executor_exception(MSG("Unrecognized node type: " + (int)node.type));
            }
        }

        if(!original)
        {
            size_t total_size = 0;
            for(auto& name : scope)
            {
                auto& var = _variables.at(name);
                auto type_it = types.find(var.type.type);
                auto variation_it = type_it->second.GetVariation(var.type.variation); 

                //call destructor
                type_it->second.destruct(type_it, variation_it, var.value);

                total_size += variation_it->second.size;

                // Remove variable 
                _variables.erase(name);
            }

            _storage.Deallocate(total_size);
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

    void Memory::Deallocate(size_t size)
    {
        if(start == nullptr) return;
        if((size_t) start - size < (size_t)data) return;

        *((size_t*)&start) -= size;
    }
}