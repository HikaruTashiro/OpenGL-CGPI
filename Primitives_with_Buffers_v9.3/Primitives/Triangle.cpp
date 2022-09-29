#include "Line.cpp"
#include "../Utils.cpp"
#include "Primitive.hpp"
#include <iostream>
#include <new>
#pragma once

class Triangle : public Primitive
{
	public:
		Line *L1, *L2, *L3;
		Triangle();
		~Triangle();
		Triangle(Point& P, Point& Q, Point& R);
		void draw(void) override;
		bool belongs(float x, float y) override;
		friend ordered_json &operator << (ordered_json &jprimitive, Triangle &T);
};

Triangle::Triangle()
{
	type = TRIANGLE;
	L1 = L2 = L3 = nullptr;
}

Triangle::Triangle(Point& P, Point& Q, Point& R)
{
	type = TRIANGLE;
	P.R = Q.R = R.R;
	P.G = Q.G = R.G;
	P.B = Q.B = R.B;
	L1 = new(std::nothrow) Line(P,Q);	
	L2 = new(std::nothrow) Line(Q,R);	
	L3 = new(std::nothrow) Line(R,P);	
}

void Triangle::draw()
{
	if(L1 != nullptr)
		L1->draw();
	if(L2 != nullptr)
		L2->draw();
	if(L3 != nullptr)
		L3->draw();
}

Triangle::~Triangle()
{
	delete L1;
	L2->P1 = nullptr;
	delete L2;
	L3->P1 = nullptr;
	L3->P2 = nullptr;
	delete L3;
}

bool Triangle :: belongs(float x, float y)
{
	bool belongs = false;
	belongs = L1->belongs(x,y) | L2->belongs(x,y) | L3->belongs(x,y);
	return belongs;
}

ordered_json &operator << (ordered_json &jprimitive, Triangle &T)
{
	ordered_json obj;
	float width, height;
	width = static_cast<float>(T.L1->P1->width);
	height = static_cast<float>(T.L1->P1->height);

	obj["p1"]["x"] = T.L1->P1->x / width;
	obj["p1"]["y"] = T.L1->P1->y / height;
	obj["p2"]["x"] = T.L2->P1->x / width;
	obj["p2"]["y"] = T.L2->P1->y / height;
	obj["p3"]["x"] = T.L3->P1->x / width;
	obj["p3"]["y"] = T.L3->P1->y / height;
	obj["cor"]["r"] = static_cast<int>(T.L3->P2->R * 250.0f);
	obj["cor"]["g"] = static_cast<int>(T.L3->P2->G * 250.0f);
	obj["cor"]["b"] = static_cast<int>(T.L3->P2->B * 250.0f);
	jprimitive.push_back(obj);

	return jprimitive;
}
