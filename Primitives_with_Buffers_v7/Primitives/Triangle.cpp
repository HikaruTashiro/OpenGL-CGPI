#include "Line.cpp"
#include "../Utils.cpp"
#pragma once

class Triangle
{
	public:
        int type;
		Point *P1, *P2, *P3;
		Triangle(){ P1 = P2 = P3 = nullptr; }
		Triangle(Point* P, Point* Q, Point* R) 
		{
			type = TRIANGLE;
			P1 = P,P2 = Q, P3 = R;
		}
		void draw();
};

void Triangle::draw()
{
	P2->R = P3->R;
	P2->G = P3->G;
	P2->B = P3->B;
	Line L(P1,P2);
	Line N(P2,P3);
	Line M(P1,P3);
	L.draw();
	N.draw();
	M.draw();
}
