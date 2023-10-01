#pragma once

#include <string>
#include <set>

#include "exception.hpp"
#include "token.hpp"

namespace ofl
{
    struct VariationInfo
    {
        size_t size;
        VariationInfo(size_t size) :size(size) { }
        
    private:
        VariationInfo();
    };

    typedef std::map<std::string, VariationInfo> VariationMap;

    struct TypeInfo
    {
        TypeInfo(const char* name, const char *dv, VariationMap info, std::set<TokenType> set)
            :variations(info), accepts(set) 
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
        bool Accepts(TokenType type)
        {
            return accepts.find(type) != accepts.end();
        }
        bool HasVariation(std::string &variation) 
        { 
            return variations.find(variation) != variations.end(); 
        }
        VariationMap::iterator GetVariation(std::string &variation) 
        { 
            return variations.find(variation);
        }
        VariationMap::iterator Default() { return variations.find(default_variation); }
    private:
        std::string default_variation;
        VariationMap variations;
        std::set<TokenType> accepts;
        TypeInfo();
    };

    typedef std::map<std::string, TypeInfo > TypeMap;

    struct TypeInstance
    {
        TypeInstance(std::string type, std::string variation) 
            :type(type), variation(variation) { }
        std::string type;
        std::string variation;

    private:
        TypeInstance();
    };
}