#include <new>
#include "Node.cpp"

#pragma once
template<class Type> class LStack
{
    protected:
        int size;
        Node<Type> *top;         

    public:
        LStack()
        {
            size = 0;
            top = nullptr;
        }
        
        ~LStack();
        void push(Type value);
        void pop(void);
        Type getTop();
        Node<Type>* getTopPointer();
        int getSize(void){return size;}
        bool isEmpty(void){return size == 0 ? true : false;}
        Type* getStack();
        void show(void);    //will not define here since it depends on the data Type used
};

template<class Type> LStack<Type> :: ~LStack()
{
    while (size > 0)
        pop();
}

template<class Type> void LStack<Type> :: push(Type value)
{
    if(top == nullptr)
        top = new Node<Type>(value);
    else
    {
        top->next = new Node<Type>(value);
        top->next->prev = top;
        top = top->next;
    }
    size++;
}

template<class Type> void LStack<Type> :: pop()
{
    Node<Type> *aux = top;
    if(aux != nullptr)
    {
        top = top->prev;
        if(top != nullptr)
            top->next = nullptr;
        delete aux;
        size--;
    }
}

template<class Type> Type LStack<Type> :: getTop()
{
    return top->value;
}

template<class Type> Node<Type>* LStack<Type> :: getTopPointer()
{
    return top;
}

template<class Type> Type* LStack<Type> :: getStack() 
{
    Type* S = new Type[size];
    Node<Type> *aux = top;  
    int i = 0;
    do
    {
        S[i++] = aux->value;
        aux = aux->prev;
    }while (aux != nullptr);

    return S;
}
