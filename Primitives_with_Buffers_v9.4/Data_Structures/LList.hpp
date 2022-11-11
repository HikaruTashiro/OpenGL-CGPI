#include <new>
#include <iostream>
#include "Node.cpp"

template<class Type> class LList
{
  protected:
    int size;
    Node<Type> *start;
    Node<Type> *end;

  public: 
    LList();
    ~LList();
    int getSize(void);
    Node<Type>* getStart(void);
    Node<Type>* getEnd(void);
    Type getTop(void);
    void add(Type item);
    Type removeStart(void);
    Type removeEnd(void);
    Type remove(Node<Type>* node_address);
    void show(void);  //Only for Test
};

template<class Type>
LList<Type> :: LList()
{
    start = end = nullptr;
    size = 0;
}

template<class Type>
LList<Type> :: ~LList()
{
    while (size > 0)
        removeStart(); 
}

template<class Type>
int LList<Type> :: getSize() 
{
    return size;
}

template<class Type>
Node<Type>* LList<Type> :: getStart()
{
    return start;
}

template<class Type>
Node<Type>* LList<Type> :: getEnd()
{
    return end;
}

template<class Type>
Type LList<Type> :: getTop()
{
    return end->value;
}

template<class Type>
void LList<Type> :: add(Type item)
{
    Node<Type>* new_Node = new(std::nothrow) Node<Type>(item);
    if(new_Node != nullptr)
    {
        if(size == 0)
            start = end = new_Node;
        else
        {
            new_Node->prev = end;
            end->next = new_Node;
            end = new_Node;
        }
        size++;
    }
}

template<class Type>
Type LList<Type> :: removeStart()
{
      Node<Type>* aux = start; 
      Type value = (Type) 0;
      if(aux != nullptr)
      {
          value = start->value;
          start = start->next;
          if(start != nullptr)
            start->prev = nullptr;
          else
            start = end = nullptr;
          delete aux;
          size--; 
      }
      return value;
}

template<class Type>
Type LList<Type> :: removeEnd()
{
      Node<Type>* aux = end; 
      Type value = (Type) 0;
      if(aux != nullptr)
      {
          value = end->value;
          end = end->prev;
          if(end != nullptr)
            end->next = nullptr;
          else
            start = end = nullptr;
          delete aux;
          size--; 
      }
      return value;
}

template<class Type>
Type LList<Type> :: remove(Node<Type> *node_address)
{
      Type value = (Type) 0;
      if(node_address != nullptr)    
      {
            if(node_address->prev != nullptr)
                node_address->prev->next = node_address->next; 
            if(node_address->next != nullptr)
                node_address->next->prev = node_address->prev;
            if(node_address == start)
                start = node_address->next;
            if(node_address == end)
                end = node_address->prev;
            value = node_address->value;
            delete node_address;
            size--;
      }
      return value;
}
