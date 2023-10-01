#pragma once

#include <string.h>
#include <set>

#include "exception.hpp"
#include "token.hpp"

namespace ofl
{
    struct VariationInfo;
    struct TypeInfo;

    typedef std::map<std::string, VariationInfo> VariationMap;
    typedef std::map<std::string, TypeInfo > TypeMap;

    typedef VariationMap::iterator VariationMap_it;
    typedef TypeMap::iterator TypeMap_it;

    typedef void (*AssignmentFunction)(TypeMap_it&, VariationMap_it&, void *, void *);
    typedef void (*PrintFunction)(TypeMap_it&, VariationMap_it&, void *);

    struct VariationInfo
    {
        size_t size;
        VariationInfo(size_t size) :size(size) { }
        
    private:
        VariationInfo();
    };

    inline void print_default(TypeMap_it& type, VariationMap_it& variation, void *ptr);
    inline void assign_default(TypeMap_it& type, VariationMap_it& variation, void *ptr, void *value);

    struct TypeInfo
    {
        TypeInfo(const char* name, const char *dv, VariationMap info, std::set<TokenType> set,
                AssignmentFunction assign = &assign_default, PrintFunction print = &print_default)
            :variations(info), accepts(set), assign(assign), print(print)
        {
            // Verify map is at least 1 large
            if(variations.size() < 1)
                throw executor_exception(MSG("Type must have at least 1 variation: " + name));

            // Verify default is in map
            auto default_it = variations.find(dv);
            if(default_it == variations.end())
                throw executor_exception(MSG("Default variation '" + dv + "' not found: " + name));
            
            default_variation = dv;
        }

        const char *AcceptString()
        {
            return to_string(*accepts.begin());
        }

        bool Accepts(TokenType type)
        {
            return accepts.find(type) != accepts.end();
        }

        bool HasVariation(std::string &variation) 
        { 
            return variations.find(variation) != variations.end(); 
        }

        VariationMap_it GetVariation(std::string &variation) 
        { 
            return variations.find(variation);
        }

        VariationMap_it DefaultVariation() { return variations.find(default_variation); }

        AssignmentFunction assign;
        PrintFunction print;
    private:
        std::string default_variation;
        VariationMap variations;
        std::set<TokenType> accepts;
        TypeInfo();
    };

    struct TypeInstance
    {
        TypeInstance(std::string type, std::string variation) 
            :type(type), variation(variation) { }
        std::string type;
        std::string variation;

    private:
        TypeInstance();
    };

    inline void assign_boolean(TypeMap_it&, VariationMap_it&, void *ptr, void *value)
    {
        /**
        * -1 = false
        * -2 = no
        * 1 = true
        * 2 = yes
        */
        if(strcmp((const char *) value, "true") == 0)
            *((int8_t*)ptr) = 1;
        else if(strcmp((const char *) value, "yes") == 0)
            *((int8_t*)ptr) = 2;
        else if(strcmp((const char *) value, "false") == 0)
            *((int8_t*)ptr) = -1;
        else if(strcmp((const char *) value, "no") == 0)
            *((int8_t*)ptr) = -2;
        else 
            *((int8_t*)ptr) = (_atoi64((const char *) value) > 0 ? 1 : -1);
    }

    inline void assign_integer(TypeMap_it& type, VariationMap_it& variation, void *ptr, void *value)
    {
         if(variation->second.size == 4)
            *((int32_t*)ptr) = atoi((const char *) value);
        else if(variation->second.size == 8)
            *((int64_t*)ptr) = _atoi64((const char *) value);
    }
    
    inline void assign_decimal(TypeMap_it& type, VariationMap_it& variation, void *ptr, void *value)
    {
         if(variation->second.size == 4)
            *((float*)ptr) = (float) atof((const char *) value);
        else if(variation->second.size == 8)
            *((double*)ptr) = atof((const char *) value);
    }

    inline void assign_string(TypeMap_it& type, VariationMap_it& variation, void *ptr, void *value)
    {

    }

    inline void assign_default(TypeMap_it& type, VariationMap_it& variation, void *ptr, void *value)
    {
        throw executor_exception(MSG("Assignment for '" + type->first + ":" + variation->first + "' not implemented."));
    }

    inline void print_boolean(TypeMap_it& type, VariationMap_it& variation, void *ptr)
    {
        int8_t temp = (*(int8_t*) ptr);
        if(temp <= 0)
            std::cout << (temp == -1 ? "false" : "no") << std::endl;
        else
            std::cout << (temp == 1 ? "true" : "yes") << std::endl;
    }

    inline void print_integer(TypeMap_it& type, VariationMap_it& variation, void *ptr)
    {
        if(variation->second.size == 4)
            std::cout << *(int32_t*) ptr << std::endl;
        else if(variation->second.size == 8)
            std::cout << *(int64_t*) ptr << std::endl;
    }

    inline void print_decimal(TypeMap_it& type, VariationMap_it& variation, void *ptr)
    {
        if(variation->second.size == 4)
            std::cout << *(float*) ptr << std::endl;
        else if(variation->second.size == 8)
            std::cout << *(double*) ptr << std::endl;
    }

    inline void print_default(TypeMap_it& type, VariationMap_it& variation, void *ptr)
    {
        std::cout << "<" << type->first << ":" << variation->first << "@" << ptr << ">" << std::endl;
    }
    /**
     * @brief void Executor::setValue(TypeMap_it &type, VariationMap_it &variation, void *ptr, const char *value)
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
        else if(type->first == "bin")
        {
            // -1 = false; -2 = no; 1 = true; 2 = yes;
            if(strcmp(value, "true") == 0)
                *((int8_t*)ptr) = 1;
            else if(strcmp(value, "yes") == 0)
                *((int8_t*)ptr) = 2;
            else if(strcmp(value, "false") == 0)
                *((int8_t*)ptr) = -1;
            else if(strcmp(value, "no") == 0)
                *((int8_t*)ptr) = -2;
            else 
                *((int8_t*)ptr) = (_atoi64(value) > 0 ? 1 : -1);
        }
        else
            throw executor_exception(MSG("Undefined type name: " + type->first));
    }
     * 
     */
}