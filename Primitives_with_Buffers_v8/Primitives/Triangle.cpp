#include "../Utils.cpp"
#include "Line.cpp"
#include "Primitive.hpp"
#include <new>
#pragma once

class Triangle : public Primitive {
public:
  Line *L1, *L2, *L3;
  Triangle();
  ~Triangle();
  Triangle(Point &P, Point &Q, Point &R);
  bool belongs(float x, float y) override;
  void draw(void) override;
  void serializeToJSON(std::string fileName) override;
};

Triangle::Triangle() {
  type = TRIANGLE;
  L1 = L2 = L3 = nullptr;
}

Triangle::Triangle(Point &P, Point &Q, Point &R) {
  type = TRIANGLE;
  P.R = Q.R = R.R;
  P.G = Q.G = R.G;
  P.B = Q.B = R.B;
  L1 = new (std::nothrow) Line(P, Q);
  L2 = new (std::nothrow) Line(Q, R);
  L3 = new (std::nothrow) Line(R, P);
}

void Triangle::draw() {
  if (L1 != nullptr)
    L1->draw();
  if (L2 != nullptr)
    L2->draw();
  if (L3 != nullptr)
    L3->draw();
}

Triangle::~Triangle() {
  delete L1;
  delete L2;
  delete L3;
}

void Triangle::serializeToJSON(std::string fileName) {}

bool Triangle ::belongs(float x, float y) {
  bool belongs = false;
  belongs = L1->belongs(x, y) | L2->belongs(x, y) | L3->belongs(x, y);
  return belongs;
}
