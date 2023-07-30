#pragma once

#include<vector>

namespace ofl
{
    enum class NodeType
    {
        Empty,
        Sequence,
        Declaration,
        Assignment,
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
        Node(NodeType type = NodeType::Empty):_type(type) { _children = new std::vector<Node>();}
        Node(Node&) = delete;
        Node(Node&& other):_type(other._type), _children(other._children) 
        { other._children = nullptr; }
        ~Node() { if(_children != nullptr) delete _children; }
    };

    class AST
    {
    public:
        static void Print(Node* root);
    };
}