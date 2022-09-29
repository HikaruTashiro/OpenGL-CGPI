#include "Point.cpp"
#include "../Utils.cpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#pragma once
void LineLower(Point *P1, Point *P2);
void LineUpper(Point *P1, Point *P2);

class Line
{
	public:	
        int type;
		Point *P1, *P2;
		Line(){type = LINE; P1 = P2 = nullptr;}
		Line(Point *P_i,Point *P_f){type = LINE; P1 = P_i; P2 = P_f;}
		void draw();
};

void Line :: draw()
{
    if(std::abs(P2->y - P1->y) < std::abs(P2->x - P1->x))
    {
        if(P1->x > P2->x)
          LineLower(P2,P1);
        else
          LineLower(P1,P2);
    }
    else
    {
        if(P1->y > P2->y)
            LineUpper(P2,P1);
        else
            LineUpper(P1,P2);
    }
}

void LineLower(Point *P1, Point *P2)
{
    GLint dx, dy, x, y, incrx, incry, incrE, incrNE;
    x = P1->x;
    y = P1->y;
    incry = incrx = P2->PointSize;
    dx = P2->x - P1->x;
    dy = P2->y - P1->y;
    Point P(x, y, P2->getWindow());
	P.PointSize = P2->PointSize;
	P.R = P2->R;
	P.G = P2->G;
	P.B = P2->B;

    if(dy < 0)
    {
        dy = -dy;
        incry = -incry;
    }

    incrE = dy << 1;
    incrNE = (dy - dx) << 1;
    GLint d = (dy << 1) - dx;

    while(x < P2->x)
    {
      if(d <= 0)
      {
          d += incrE;
          x += incrx;
      }
      else
      {
          d += incrNE;
          x += incrx;
          y += incry;
      }
      P.x = x;
      P.y = y;
      P.draw();
    }
}

void LineUpper(Point *P1, Point *P2)
{
    GLint dx, dy, x, y, incrx,incry, incrE, incrNE;
    x = P1->x;
    y = P1->y;
    incry = incrx = P2->PointSize;
    dx = P2->x - P1->x;
    dy = P2->y - P1->y;
    Point P(x, y, P2->getWindow());
	P.PointSize = P2->PointSize;
	P.R = P2->R;
	P.G = P2->G;
	P.B = P2->B;

    if(dx < 0)
    {
        dx = -dx;
        incrx = -incrx;
    }

    incrE = dx << 1;
    incrNE = (dx - dy) << 1;
    GLint d = (dx << 1) - dy;

    while(y < P2->y)
    {
        if(d <= 0)
        {
            d += incrE;
            y += incry;
        }
        else
        {
            d += incrNE;
            x += incrx;
            y += incry;
        }
        P.x = x;
        P.y = y;
        P.draw();
    }
}
