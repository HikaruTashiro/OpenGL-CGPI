#include "../Utils.cpp"
#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <new>

#pragma once
class Rectangle : public Primitive {
public:
  Line *L1, *L2, *L3, *L4;
  Rectangle();
  Rectangle(Point &P, Point &Q);
  ~Rectangle();
  void draw(void) override;
  bool belongs(float x, float y) override;
  void serializeToJSON(std::string fileName) override;
};

Rectangle ::Rectangle() {
  type = RECTANGLE;
  L1 = L2 = L3 = L4 = nullptr;
}

Rectangle ::Rectangle(Point &P, Point &Q) {
  type = RECTANGLE;
  Point *R, *S;
  R = new (std::nothrow) Point(P.x, Q.y, P.getWindow());
  S = new (std::nothrow) Point(Q.x, P.y, P.getWindow());

  if (R != nullptr && S != nullptr) {
    R->R = S->R = P.R;
    R->G = S->G = P.G;
    R->B = S->B = P.B;
    L1 = new (std::nothrow) Line(P, *R);
    L2 = new (std::nothrow) Line(Q, *R);
    L3 = new (std::nothrow) Line(P, *S);
    L4 = new (std::nothrow) Line(Q, *S);
  }
}

Rectangle ::~Rectangle() {
  delete L1;
  delete L2;
  delete L3;
  delete L4;
}

void Rectangle ::draw() {
  if (L1 != nullptr)
    L1->draw();
  if (L2 != nullptr)
    L2->draw();
  if (L3 != nullptr)
    L3->draw();
  if (L4 != nullptr)
    L4->draw();
}

void Rectangle::serializeToJSON(std::string fileName) {}

bool Rectangle ::belongs(float x, float y) {
  bool belongs = false;
  belongs = L1->belongs(x, y) | L2->belongs(x, y) | L3->belongs(x, y) |
            L4->belongs(x, y);
  return belongs;
}
