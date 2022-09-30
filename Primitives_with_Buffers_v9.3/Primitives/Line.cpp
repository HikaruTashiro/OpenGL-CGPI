#include "../Utils.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <utility>

#pragma once

void LineLower(Point &P1, Point &P2);
void LineUpper(Point &P1, Point &P2);

class Line : public Primitive {
public:
  Point *P1, *P2;
  Line();
  Line(Point &Pi, Point &Pf);
  ~Line();
  bool belongs(float x, float y) override;
  void draw(void) override;
  void translate(float x, float y) override;
  void scale(float r) override;
  void newPoints(std::pair<float, float> P1, std::pair<float, float> P2) {
    this->P1->x = P1.first;
    this->P1->y = P1.second;
    this->P2->x = P2.first;
    this->P2->y = P2.second;
  }
  void rotateItself(float rad) override;
  std::pair<float, float> getMidPoint();
  // std::pair<float, float> rotateAround(float rad, std::pair<float, float> p,
  //                                     std::pair<float, float> q);
  friend ordered_json &operator<<(ordered_json &jprimitive, Line &L);
};

Line::Line() {
  type = LINE;
  P1 = P2 = nullptr;
}

Line::Line(Point &Pi, Point &Pf) {
  type = LINE;
  P1 = &Pi;
  P2 = &Pf;
}

Line::~Line() {
  if (P1 != nullptr)
    delete P1;
  if (P2 != nullptr)
    delete P2;
}

void Line ::draw() {
  if (std::abs(P2->y - P1->y) < std::abs(P2->x - P1->x)) {
    if (P1->x > P2->x)
      LineLower(*P2, *P1);
    else
      LineLower(*P1, *P2);
  } else {
    if (P1->y > P2->y)
      LineUpper(*P2, *P1);
    else
      LineUpper(*P1, *P2);
  }
}

void LineLower(Point &P1, Point &P2) {
  GLint dx, dy, x, y, incrx, incry, incrE, incrNE;
  x = P1.x;
  y = P1.y;
  incry = incrx = P2.PointSize;
  dx = P2.x - P1.x;
  dy = P2.y - P1.y;
  Point P(x, y, P2.getWindow());
  P.PointSize = P2.PointSize;
  P.R = P2.R;
  P.G = P2.G;
  P.B = P2.B;
  P.setColor();
  P.setSize();

  if (dy < 0) {
    dy = -dy;
    incry = -incry;
  }

  incrE = dy << 1;
  incrNE = (dy - dx) << 1;
  GLint d = (dy << 1) - dx;

  while (x < P2.x) {
    if (d <= 0) {
      d += incrE;
      x += incrx;
    } else {
      d += incrNE;
      x += incrx;
      y += incry;
    }
    P.x = x;
    P.y = y;
    P.draw();
  }
}

void LineUpper(Point &P1, Point &P2) {
  GLint dx, dy, x, y, incrx, incry, incrE, incrNE;
  x = P1.x;
  y = P1.y;
  incry = incrx = P2.PointSize;
  dx = P2.x - P1.x;
  dy = P2.y - P1.y;
  Point P(x, y, P2.getWindow());
  P.PointSize = P2.PointSize;
  P.R = P2.R;
  P.G = P2.G;
  P.B = P2.B;

  if (dx < 0) {
    dx = -dx;
    incrx = -incrx;
  }

  incrE = dx << 1;
  incrNE = (dx - dy) << 1;
  GLint d = (dx << 1) - dy;

  while (y < P2.y) {
    if (d <= 0) {
      d += incrE;
      y += incry;
    } else {
      d += incrNE;
      x += incrx;
      y += incry;
    }
    P.x = x;
    P.y = y;
    P.draw();
  }
}

bool Line ::belongs(float x, float y) {
  bool belongs = false;
  float epsilon = 20.0f;
  float deltaLx = P2->x - P1->x;
  float deltaLy = P2->y - P1->y;
  float deltax = x - P1->x;
  float deltay = y - P1->y;
  float distance = std::abs(deltay * deltaLx - deltax * deltaLy);
  distance /= std::sqrt(deltaLx * deltaLx + deltaLy * deltaLy);

  if (distance < epsilon)
    belongs = true;

  return belongs;
}

void Line::translate(float x, float y) {
  this->P1->x += x;
  this->P2->x += x;
  this->P1->y += y;
  this->P2->y += y;
}

void Line::scale(float r) {
  this->P1->x *= r;
  this->P1->y *= r;
  this->P2->x *= r;
  this->P2->y *= r;
}

std::pair<float, float> Line::getMidPoint() {
  float midX = (this->P1->x + this->P2->x) / 2.0;
  float midY = (this->P1->y + this->P2->y) / 2.0;
  return std::make_pair(midX, midY);
}

std::pair<float, float> rotateAround(float deg, std::pair<float, float> p,
                                     std::pair<float, float> q) {
  float rad = deg * (M_PI / 180.0f);
  float newX = p.first * std::cos(rad) + (-1.0f * p.second * std::sin(rad)) +
               q.first * (1.0f - std::cos(rad)) + q.second * std::sin(rad);
  float newY = p.first * std::sin(rad) + p.second * std::cos(rad) +
               q.second * (1.0f - std::cos(rad)) - q.first * std::sin(rad);
  return std::make_pair(newX, newY);
}

void Line::rotateItself(float rad) {
  std::pair<float, float> pP1, pP2;
  pP1 = rotateAround(rad, std::make_pair(this->P1->x, this->P1->y),
                     getMidPoint());
  pP2 = rotateAround(rad, std::make_pair(this->P2->x, this->P2->y),
                     getMidPoint());
  this->P1->x = pP1.first;
  this->P1->y = pP1.second;
  this->P2->x = pP2.first;
  this->P2->y = pP2.second;
}

ordered_json &operator<<(ordered_json &jprimitive, Line &L) {
  ordered_json obj;

  obj["p1"]["x"] = L.P1->x / static_cast<float>(L.P1->width);
  obj["p1"]["y"] = L.P1->y / static_cast<float>(L.P1->height);
  obj["p2"]["x"] = L.P2->x / static_cast<float>(L.P1->width);
  obj["p2"]["y"] = L.P2->y / static_cast<float>(L.P1->height);
  obj["cor"]["r"] = static_cast<int>(L.P2->R * 250.0f);
  obj["cor"]["g"] = static_cast<int>(L.P2->G * 250.0f);
  obj["cor"]["b"] = static_cast<int>(L.P2->B * 250.0f);

  jprimitive.push_back(obj);

  return jprimitive;
}
