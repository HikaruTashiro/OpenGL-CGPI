#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include "../Data_Structures/Node.cpp"
#include <cstdlib>
#include <new>
#include <vector>
#pragma once

class PolygonLine : public Primitive 
{
    public:
        std::vector<Line*> lines; //Dynamic Array of pointers to Lines
        PolygonLine();
        ~PolygonLine();
        void add(Point* P);
        void addFirst(Point* P, Point*Q);

        /*overrides*/
        void draw() override;
        bool belongs(float x, float y) override;
        void translate(float x, float y) override;
        void scale(float k) override;
        void rotate(float theta) override;

		/*JSON*/
        friend ordered_json &operator << (ordered_json &jprimitive, PolygonLine &Pl);
};

PolygonLine :: PolygonLine()
{
    type = PLINE; 
}

void PolygonLine :: draw()
{
    int size = lines.size();

    for (int i = 0; i < size; i++)
        lines[i]->draw();
}

void PolygonLine :: add(Point *P)
{
    int i = lines.size() - 1;
    Line* L = new(std::nothrow) Line(*lines[i]->P2, *P);
    if(L != nullptr)
        lines.push_back(L);
}

PolygonLine :: ~PolygonLine()
{
    int n = lines.size();
    delete lines[n - 1]->P2;
    while(lines.size() > 0)
    {   
        n = lines.size() - 1;
        lines[n]->P2 = nullptr; 
        delete lines[n];
        lines.pop_back();
    }
}

void PolygonLine :: addFirst(Point *P, Point *Q)
{
    lines.push_back(new Line(*P,*Q));
}

bool PolygonLine :: belongs(float x, float y)
{
    bool belongs = false;
    unsigned long i = 0;
    while (i < lines.size() && !belongs)
    {
        belongs |= lines[i]->belongs(x,y);
        i++;
    }
    
    return belongs;
}

void PolygonLine :: translate(float x, float y)
{
    Line* &L = lines.back();
    if(type == POLYGON)
        lines.pop_back();   //apply PolyLine and then add on the end


    unsigned long size = lines.size();

    for (unsigned long i = 0; i < size; i += 2)
        lines[i]->translate(x,y);
    if(size % 2 == 0)
        lines[size - 1]->P2->translate(x,y);

    if(type == POLYGON)
        lines.push_back(L);
}

void PolygonLine :: scale(float k)
{
    Line* &L = lines.back();
    if(type == POLYGON)
        lines.pop_back();

    unsigned long size = lines.size();

    for (unsigned long i = 0; i < size; i += 2)
        lines[i]->scale(k);
    if(size % 2 == 0)
        lines[size - 1]->P2->scale(k);

    if(type == POLYGON)
        lines.push_back(L);
}

void PolygonLine :: rotate(float theta)
{
    Line* &L = lines.back();
    if(type == POLYGON)
        lines.pop_back();

    unsigned long size = lines.size();

    for (unsigned long i = 0; i < size; i += 2)
        lines[i]->rotate(theta);
    if(size % 2 == 0)
        lines[size - 1]->P2->rotate(theta);

    if(type == POLYGON)
        lines.push_back(L);
}

ordered_json &operator << (ordered_json &jprimitive, PolygonLine &Pl)
{
	ordered_json obj, aux;
    unsigned long i, size;
    float width, height;
    Line* auxL;
    width  = static_cast<float>(Pl.lines[0]->P1->width);
    height = static_cast<float>(Pl.lines[0]->P1->height);
    size = Pl.lines.size();

    if(Pl.getType() == POLYGON)
    {
        auxL = Pl.lines[size - 1];
        Pl.lines.pop_back();
    }

    for (i = 0; i < size; i++)
    {
        aux["x"] = Pl.lines[i]->P1->x / width;
        aux["y"] = Pl.lines[i]->P1->y / height;
        obj["ponto"].push_back(aux); 
    }
    i--;
    aux["x"] = Pl.lines[i]->P2->x / width;
    aux["y"] = Pl.lines[i]->P2->y / height;
    obj["ponto"].push_back(aux); 

    if(Pl.getType() == POLYGON)
        Pl.lines.push_back(auxL);

    obj["cor"]["r"] = static_cast<int>(Pl.lines[i]->P1->R * 250.0f);
    obj["cor"]["g"] = static_cast<int>(Pl.lines[i]->P1->G * 250.0f);
    obj["cor"]["b"] = static_cast<int>(Pl.lines[i]->P1->B * 250.0f);
    jprimitive.push_back(obj);
    return jprimitive;
}
