#include "Line.cpp"
#include "../Utils.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <new>
#include <string>

#pragma once
class Rectangle : public Primitive
{
    public:
        Line *L1, *L2, *L3, *L4;
        Rectangle();
        Rectangle(Point &P, Point &Q);
        ~Rectangle();

        /*overrides*/
		void draw(void) override;
        bool belongs(float x, float y) override;
        void translate(float x, float y) override;
        void scale(float k) override;
        void rotate(float theta) override;

        friend ordered_json &operator << (ordered_json &jprimitive, Rectangle &R);
};

Rectangle :: Rectangle()
{
    type = RECTANGLE;
    L1 = L2 = L3 = L4 = nullptr;
}

Rectangle :: Rectangle(Point &P, Point &Q)
{
    type = RECTANGLE;
    Point *R, *S;
    R = new(std::nothrow) Point(P.x, Q.y, P.getWindow());
    S = new(std::nothrow) Point(Q.x, P.y, P.getWindow());
    
    if(R != nullptr && S != nullptr)
    {
        R->R = S->R = P.R;
        R->G = S->G = P.G;
        R->B = S->B = P.B;
        L1 = new(std::nothrow) Line(P,*R);
        L2 = new(std::nothrow) Line(Q,*R);
        L3 = new(std::nothrow) Line(P,*S);
        L4 = new(std::nothrow) Line(Q,*S);
    }
}

Rectangle :: ~Rectangle()
{
    if(L1 != nullptr)
        delete L1;
    if(L2 != nullptr)
    {
        L2->P1 = L2->P2 = nullptr;
        delete L2;
    }
    if(L3 != nullptr)
    {
        L3->P1 = L3->P2 = nullptr;
        delete L3;
    }
    if(L4 != nullptr)
        delete L4;
}

void Rectangle :: draw()
{
    if(L1 != nullptr)
        L1->draw();
    if(L2 != nullptr)
        L2->draw();
    if(L3 != nullptr)
        L3->draw();
    if(L4 != nullptr)
        L4->draw();
}

bool Rectangle :: belongs(float x, float y)
{
	bool belongs = false;
	belongs = L1->belongs(x,y) | L2->belongs(x,y) | L3->belongs(x,y) | L4->belongs(x,y);
	return belongs;
}

void Rectangle :: translate(float x, float y)
{
    L1->translate(x,y);
    L4->translate(x,y);
}

void Rectangle :: scale(float k)
{
    L1->scale(k);
    L4->scale(k);
}

void Rectangle :: rotate(float theta)
{
    L1->rotate(theta); 
    L4->rotate(theta); 
}

ordered_json &operator << (ordered_json &jprimitive, Rectangle &R)
{
	ordered_json obj;
    float width, height;
    width = static_cast<float>(R.L1->P1->width);
    height = static_cast<float>(R.L1->P1->height);
	obj["p1"]["x"] = R.L1->P1->x / (width);
	obj["p1"]["y"] = R.L1->P1->y / (height);
	obj["p2"]["x"] = R.L2->P1->x / (width);
	obj["p2"]["y"] = R.L2->P1->y / (height);
	obj["cor"]["r"] = static_cast<int>(R.L2->P1->R * 250.0f);
	obj["cor"]["g"] = static_cast<int>(R.L2->P1->G * 250.0f);
	obj["cor"]["b"] = static_cast<int>(R.L2->P1->B * 250.0f);
	jprimitive.push_back(obj);

    return jprimitive;
}
