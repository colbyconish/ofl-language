#include "lexer.hpp"

namespace ofl
{
    std::set<std::string> Lexer::keywords = 
    {
        "print"
    };

    Lexer::Lexer() { }

    size_t Lexer::checkForFunctionCall(const std::string &name, TokenList &tokens, Node* parent, size_t pos)
    {
        size_t count = 2;
        if(tokens[pos+1].Type() != TokenType::Identifier)
            throw executor_exception(MSG("Expected identifier but found: " + tokens[pos+1]));

        // Check for semicolon
        void* ch = tokens[pos+2].Data();
        if(tokens[pos+2].Type() != TokenType::Delemiter)
            throw lexer_exception(MSG("Expected Delemiter but found: " + tokens[pos+2]));
        else if(*((char*) &ch) != ';')
            throw lexer_exception(MSG("Expected ';' but found: '" + (char*) &ch + "'"));

        // Create invocation node
        auto& node = parent->_children->emplace_back(NodeType::Invocation);

        auto& func = node._children->emplace_back(NodeType::Variable);
        func._data = new std::string(name);

        auto& arg = node._children->emplace_back(NodeType::Variable);
        arg._data = new std::string(*(std::string *)(tokens[pos+1].Data()));

        return count;
    }

    size_t Lexer::checkForAssignment(TypeInfo &type, TokenList &tokens, Node* parent, size_t pos)
    {
        static std::string name;
        static Token* value;

        // Check for assignment name
        if(tokens[pos+1].Type() == TokenType::Identifier)
            name = *((std::string*) tokens[pos+1].Data());
        else
            throw lexer_exception(MSG("Expected indentifier but found: " + tokens[pos+1]));
        
        // Check for assignment operator
        auto op = tokens[pos+2].Data();
        if(tokens[pos+2].Type() != TokenType::Operator)
            throw lexer_exception(MSG("Expected Operator but found: " + tokens[pos+2]));
        else if(op != (void*) Lexer::EQUALS_OP)
            throw lexer_exception(MSG("Expected '=' but found: '" + (char*) &op + "'"));
        
        // Check for value
        if(tokens[pos+3].Type() == TokenType::Literal)
            value = &tokens[pos+3];
        else
            throw lexer_exception(MSG("Expected Literal but found: " + tokens[pos+3]));

        if(!type.Accepts(value->Type()))
            throw lexer_exception(MSG("Wrong value in assignment: " + *((std::string*)value->Data())));
        
        // Check for semicolon
        void* ch = tokens[pos+4].Data();
        if(tokens[pos+4].Type() != TokenType::Delemiter)
            throw lexer_exception(MSG("Expected Delemiter but found: " + tokens[pos+4]));
        else if(*((char*) &ch) != ';')
            throw lexer_exception(MSG("Expected ';' but found: '" + (char*) &ch + "'"));

        // Create assignment node
        auto& node = parent->_children->emplace_back(NodeType::Declaration);

        auto& typ = node._children->emplace_back(NodeType::Type);
        typ._data = &type;

        auto& var = node._children->emplace_back(NodeType::Variable);
        var._data = new std::string(name);

        auto& val = node._children->emplace_back(NodeType::Literal);
        val._data = new std::string(*(std::string*) value->Data());
        
        return 4;
    }

    Node Lexer::Lex(std::map<std::string, TypeInfo>& types)
    {
        Node ast = Node(NodeType::Sequence);

        for(size_t i = 0;i < _tokens.size();i++)
        {
            _pos++;

            auto& token = _tokens[i];
            switch(token.Type())
            {
                case TokenType::Identifier:
                {
                    // Check if identifier is a type
                    auto it_t = types.find(*((std::string*) token.Data()));
                    if(it_t != types.end())
                    {
                        i += checkForAssignment(it_t->second, _tokens, &ast, i);
                        continue;
                    }

                    // Check if identifier is a keyword
                    auto it_k = keywords.find(*((std::string*) token.Data()));
                    if(it_k != keywords.end())
                    {
                        i += checkForFunctionCall(*it_k, _tokens, &ast, i);
                        continue;
                    }

                    // Identifier is a variable

                    throw lexer_exception(MSG("Undefined indentifier: " + token));
                    break;
                }
                default:
                    if(token.Type() == TokenType::ENDOFFILE)
                        return ast;
                    else
                        throw lexer_exception(MSG("Unrecognized token: " + token));
                    break;
            }
        }

        _tokens.clear();
        return std::move(ast);
    }
}