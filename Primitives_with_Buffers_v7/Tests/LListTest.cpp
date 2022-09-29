#include "../LList.hpp"
#include <iostream>

template<> void LList<int> :: show()
{
    Node<int>* aux = start;
    if(size != 0)
    {
        do
        {
            std::cout<<aux->value<<'\n'; 
            aux = aux->next;
        }while (aux != nullptr);
    }
    else
        std::cout<<"Empty Linked List"<<'\n';
}

int main()
{
    LList<int> Pog;
    std::cout<<"Pog has Size = "<< Pog.getSize()<<'\n';
    Pog.show();
    Pog.add(5);
    Pog.add(1);
    Pog.add(9);
    Pog.add(10);
    Pog.add(20);
    Pog.add(69);
    Pog.add(2);
    std::cout<<"Pog has Size = "<< Pog.getSize()<<'\n';
    Pog.show();
    Pog.removeEnd();
    Pog.removeStart();
    std::cout<<"Pog has Size = "<< Pog.getSize()<<'\n';
    Pog.show(); }
