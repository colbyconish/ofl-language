#pragma once

#include <string>
#include <vector>

#include "type.hpp"

namespace ofl
{
    enum class NodeType
    {
        Empty,
        Sequence,
        Declaration,
        Assignment,
        Type,
        Variable,
        Literal,
        Equatiuon,
        Invocation,
        Arguments,
        Repeat
    };

    struct Node
    {
        NodeType type;
        std::vector<Node>* children;
        void *data = nullptr;
        Node(NodeType type = NodeType::Empty, void *data = nullptr)
            :type(type), data(data), children(new std::vector<Node>()) { }
        Node(Node&) = delete;
        Node(Node&& other):type(other.type), children(other.children), data(other.data)
        {other.children = nullptr; other.data = nullptr;}
        ~Node() 
        {
            if(children != nullptr) delete children;
            if(data != nullptr) 
            {
                if(type == NodeType::Literal || type == NodeType::Variable)
                    delete (std::string *) data;
                else if(type == NodeType::Type)
                    delete (TypeInstance *) data;
                else if(type == NodeType::Sequence)
                    delete (Node *) data;
                else
                    std::cout << "Memory leak possible with node type: " << (int)type << std::endl;
            }
        }
    };

    class AST
    {
    public:
        static void Print(Node* root);
    };
}