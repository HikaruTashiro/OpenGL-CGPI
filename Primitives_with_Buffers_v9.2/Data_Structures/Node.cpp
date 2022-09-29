#pragma once
template <class NodeType> class Node
{
  public:
    NodeType value;
    Node *next;
    Node *prev;

    Node(NodeType value)
    {
        this->value = value;
        next = nullptr;
        prev = nullptr;
    }

    //~Node()
    //{
    //    std::cout<<"Removing Node: "<<value<<'\n';
    //}
};
