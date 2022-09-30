#include "../Utils.cpp"
#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <new>
#include <string>
#include <utility>

#pragma once
class Rectangle : public Primitive {
public:
  Line *L1, *L2, *L3, *L4;
  Rectangle();
  Rectangle(Point &P, Point &Q);
  ~Rectangle();
  void draw(void) override;
  bool belongs(float x, float y) override;
  void translate(float x, float y) override;
  void scale(float r) override;
  void rotateItself(float deg) override;
  std::pair<float, float> getMidPoint();
  friend ordered_json &operator<<(ordered_json &jprimitive, Rectangle &R);
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
  if (L1 != nullptr)
    delete L1;
  if (L2 != nullptr) {
    L2->P1 = L2->P2 = nullptr;
    delete L2;
  }
  if (L3 != nullptr) {
    L3->P1 = L3->P2 = nullptr;
    delete L3;
  }
  if (L4 != nullptr)
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

bool Rectangle ::belongs(float x, float y) {
  bool belongs = false;
  belongs = L1->belongs(x, y) | L2->belongs(x, y) | L3->belongs(x, y) |
            L4->belongs(x, y);
  return belongs;
}

void Rectangle::translate(float x, float y) {
  L1->translate(x, y);
  L2->translate(x, y);
  L3->translate(x, y);
  L4->translate(x, y);
}

void Rectangle::scale(float r) {
  L1->scale(r);
  L2->scale(r);
  L3->scale(r);
  L4->scale(r);
}

std::pair<float, float> Rectangle::getMidPoint() {
  float midX =
      this->L1->P1->x + this->L2->P1->x + this->L3->P1->x + this->L4->P1->x;
  float midY =
      this->L1->P1->y + this->L2->P1->y + this->L3->P1->y + this->L4->P1->y;
  return std::make_pair(midX / 4.0f, midY / 4.0f);
}

// SO MUCH WORK
void Rectangle::rotateItself(float deg) {
  std::pair<float, float> midPoint = this->getMidPoint();

  std::pair<float, float> newL1P1 = rotateAround(
      deg, std::make_pair(this->L1->P1->x, this->L1->P1->y), midPoint);
  std::pair<float, float> newL1P2 = rotateAround(
      deg, std::make_pair(this->L1->P2->x, this->L1->P2->y), midPoint);

  std::pair<float, float> newL2P1 = rotateAround(
      deg, std::make_pair(this->L2->P1->x, this->L2->P1->y), midPoint);
  std::pair<float, float> newL2P2 = rotateAround(
      deg, std::make_pair(this->L2->P2->x, this->L2->P2->y), midPoint);

  std::pair<float, float> newL3P1 = rotateAround(
      deg, std::make_pair(this->L3->P1->x, this->L3->P1->y), midPoint);
  std::pair<float, float> newL3P2 = rotateAround(
      deg, std::make_pair(this->L3->P2->x, this->L3->P2->y), midPoint);

  std::pair<float, float> newL4P1 = rotateAround(
      deg, std::make_pair(this->L4->P1->x, this->L4->P1->y), midPoint);
  std::pair<float, float> newL4P2 = rotateAround(
      deg, std::make_pair(this->L4->P2->x, this->L4->P2->y), midPoint);

  this->L1->newPoints(newL1P1, newL1P2);
  this->L2->newPoints(newL2P1, newL2P2);
  this->L3->newPoints(newL3P1, newL3P2);
  this->L4->newPoints(newL4P1, newL4P2);
}

ordered_json &operator<<(ordered_json &jprimitive, Rectangle &R) {
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
