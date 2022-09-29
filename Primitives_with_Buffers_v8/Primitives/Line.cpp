#include "../Utils.cpp"
#include "../json/json.hpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

#pragma once

void LineLower(Point &P1, Point &P2);
void LineUpper(Point &P1, Point &P2);

class Line : public Primitive {
public:
  Point *P1, *P2;
  Line();
  Line(Point &Pi, Point &Pf);
  virtual ~Line();
  virtual void serializeToJSON(std::string fileName) override;
  bool belongs(float x, float y) override;
  void draw(void) override;
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

void Line::serializeToJSON(std::string fileName) {
  std::ofstream file(fileName, std::ios::app);
  using nlohmann::json;
  json obj;
  obj["P1"]["x"] = this->P1->x;
  obj["P1"]["y"] = this->P1->y;
  obj["P2"]["x"] = this->P2->x;
  obj["P2"]["y"] = this->P2->y;
  obj["color"]["r"] = this->P1->R;
  obj["color"]["g"] = this->P1->G;
  obj["color"]["b"] = this->P1->B;
  obj["type"] = this->type;
  std::cout << obj.dump() << "\n";
  file << obj.dump();
  file << "\n";
  file.close();
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
