#pragma once

#include <string>
#include <vector>

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
        Arguments
    };

    struct Node
    {
        NodeType _type;
        std::vector<Node>* _children;
        void  *_data = nullptr;
        Node(NodeType type = NodeType::Empty, void *data = nullptr)
            :_type(type), _data(data), _children(new std::vector<Node>()) { }
        Node(Node&) = delete;
        Node(Node&& other):_type(other._type), _children(other._children), _data(other._data)
        {other._children = nullptr; other._data = nullptr;}
        ~Node() 
        {
            if(_children != nullptr) delete _children;
            if(_data != nullptr) 
                if(_type == NodeType::Literal || _type == NodeType::Variable)
                    delete (std::string *) _data;
        }
    };

    class AST
    {
    public:
        static void Print(Node* root);
    };
}