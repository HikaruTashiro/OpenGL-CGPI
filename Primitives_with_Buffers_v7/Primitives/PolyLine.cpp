#include "Line.cpp"
#include "../Utils.cpp"
#include "Point.cpp"
#include <cstdlib>
#include <new>
#include <vector>
#pragma once

class PolygonLine
{
    public:
        int type;
        std::vector<Line*> lines; //Dinamic Array of Points
        PolygonLine();
        ~PolygonLine();
        void draw();
        void add(Point* P);
        void addFirst(Point* P, Point*Q);
};

PolygonLine::PolygonLine()
{
    type = PLINE; 
}

void PolygonLine::draw()
{
    int size = lines.size();

    for (int i = 0; i < size; i++)
        lines[i]->draw();
}

void PolygonLine::add(Point *P)
{
    int i = lines.size() - 1;
    Line* L = new(std::nothrow) Line(lines[i]->P2, P);
    if(L != nullptr)
        lines.push_back(L);
}

PolygonLine::~PolygonLine()
{
    while(lines.size() != 0)
    {   
        free(lines[lines.size() - 1]);
        lines.pop_back();
    }
}

void PolygonLine::addFirst(Point* P, Point*Q)
{
    lines.push_back(new Line(P,Q));
}
