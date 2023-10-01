#include "lexer.hpp"

namespace ofl
{
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
            throw lexer_exception(MSG("Expected ';' but found: '" + (char*) (&ch) + "'"));

        // Create invocation node
        auto& node = parent->children->emplace_back(NodeType::Invocation);

        auto& func = node.children->emplace_back(NodeType::Variable);
        func.data = new std::string(name);

        auto& arg = node.children->emplace_back(NodeType::Variable);
        arg.data = new std::string(*(std::string *)(tokens[pos+1].Data()));

        return count;
    }

    size_t Lexer::checkForAssignment(TypeMap::iterator &type_it, TokenList &tokens, Node* parent, size_t pos)
    {
        std::string variation = type_it->second.DefaultVariation()->first;
        std::string name;
        Token* value;

        size_t count = 1;

        // Check for type variation sequence
        if(tokens[pos+count].Type() == TokenType::Operator)
        {
            // Verify that the operator is the variator operator ':'
            auto op = tokens[pos+count].Data();
            if(op != (void*) COLON_OP)
                throw lexer_exception(MSG("Expected ':' but found: '" + (char*) &op + "'"));

            // Check for variation string
            if(tokens[pos+count+1].Type() == TokenType::Identifier || tokens[pos+count+1].Type() == TokenType::NumberLiteral)
                variation = *((std::string*) tokens[pos+count+1].Data());
            else
                throw lexer_exception(MSG("Expected indentifier or operator but found: "), tokens[pos+count+1]);  

            // Verify that variation exists
            if(!type_it->second.HasVariation(variation))
                    throw lexer_exception(MSG("Received undefined variation of type '" + type_it->first + "': " + variation));

            count+=2;
        }
        
        // Check for assignment name
        if(tokens[pos+count].Type() == TokenType::Identifier)
            name = *((std::string*) tokens[pos+count].Data());
        else
            throw lexer_exception(MSG("Expected indentifier or operator but found: "), tokens[pos+count]);
        count++;

        // Check for assignment operator
        auto op = tokens[pos+count].Data();
        if(tokens[pos+count].Type() != TokenType::Operator)
            throw lexer_exception(MSG("Expected Operator but found: " + tokens[pos+count]));
        else if(op != (void*) EQUALS_OP)
            throw lexer_exception(MSG("Expected '=' but found: '" + (char*) &op + "'"));
        count++;
        
        // Check for value
        if(type_it->second.Accepts(tokens[pos+count].Type()))
            value = &tokens[pos+count];
        else
            throw lexer_exception(MSG("Expected '" + type_it->second.AcceptString() + "' but found: " + tokens[pos+count]));
        count++;

        // Check for semicolon
        void* ch = tokens[pos+count].Data();
        if(tokens[pos+count].Type() != TokenType::Delemiter)
            throw lexer_exception(MSG("Expected Delemiter but found: " + tokens[pos+count]));
        else if(*((char*) &ch) != ';')
            throw lexer_exception(MSG("Expected ';' but found: '" + (char*) &ch + "'"));

        // Create assignment node
        auto& node = parent->children->emplace_back(NodeType::Declaration);

        auto& typ = node.children->emplace_back(NodeType::Type);
        typ.data = new TypeInstance(type_it->first, type_it->second.GetVariation(variation)->first);

        auto& var = node.children->emplace_back(NodeType::Variable);
        var.data = new std::string(name);

        auto& val = node.children->emplace_back(NodeType::Literal);
        val.data = new std::string(*(std::string*) value->Data());
        
        return count;
    }

    Node Lexer::Lex(TypeMap& types)
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
                        i += checkForAssignment(it_t, _tokens, &ast, i);
                        continue;
                    }

                    // Identifier is a variable

                    throw lexer_exception(MSG("Undefined indentifier: " + token));
                    break;
                }
                case TokenType::Keyword:
                {
                    // Check if keyword is function
                    i += checkForFunctionCall(*(std::string*) token.Data(), _tokens, &ast, i);
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