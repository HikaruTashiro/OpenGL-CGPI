#include <iostream>
#include <GL/glew.h>
#include "../Data_Structures/LStack.hpp"

template<> void LStack<GLfloat> :: show()
{
	Node<GLfloat> * aux = top;
	do
	{
		std::cout<<aux->value<<'\n';
		aux = aux->prev;
	}while (aux != nullptr);
}

int main()
{
	LStack<GLfloat> Buffer;
	Buffer.push(3.14);
	Buffer.push(2.14);
	Buffer.push(1.14);
	Buffer.show();
}
