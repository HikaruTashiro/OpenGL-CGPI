#include "Line.cpp"
#include "../Utils.cpp"

#pragma once
class Rectangle
{
    public:
        int type;
        Point *P1, *P2;
        Rectangle(){ P1 = P2 = nullptr; }
        Rectangle(Point* P, Point* Q)
        {
            type = RECTANGLE;
            P1 = P;
            P2 = Q;
        }
		void draw();
};

void Rectangle :: draw()
{
    GLFWwindow* window = P1->getWindow();
    Point P(0.0f,0.0f,window);
    P.R = P2->R;
    P.G = P2->G;
    P.B = P2->B;
    P.PointSize = P2->PointSize;

    P.x = P1->x; 
    P.y = P2->y;

    Line L(P1,&P);
    L.draw();   //P1 to P

    L.P1 = P2;
    L.draw();   //P2 to P

    P.x = P2->x; 
    P.y = P1->y;

    L.draw();   //P2 to P(changed)
    L.P1 = P1;
    L.draw();   //P1 to P(changed)
}
