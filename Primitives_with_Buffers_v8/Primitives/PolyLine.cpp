#include "../Utils.cpp"
#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <cstdlib>
#include <new>
#include <vector>
#pragma once

class PolygonLine : public Primitive {
public:
  std::vector<Line *> lines; // Dinamic Array of Points
  PolygonLine();
  ~PolygonLine();
  void draw() override;
  void add(Point *P);
  void addFirst(Point *P, Point *Q);
  bool belongs(float x, float y) override;
  void serializeToJSON(std::string fileName) override;
};

PolygonLine ::PolygonLine() { type = PLINE; }

void PolygonLine ::draw() {
  int size = lines.size();

  for (int i = 0; i < size; i++)
    lines[i]->draw();
}

void PolygonLine ::add(Point *P) {
  int i = lines.size() - 1;
  Line *L = new (std::nothrow) Line(*lines[i]->P2, *P);
  if (L != nullptr)
    lines.push_back(L);
}

PolygonLine ::~PolygonLine() {
  int n;
  while (lines.size() != 0) {
    n = lines.size() - 1;
    delete lines[n]->P2;
    delete lines[n];
    lines.pop_back();
  }
}

void PolygonLine::serializeToJSON(std::string fileName) {}

void PolygonLine ::addFirst(Point *P, Point *Q) {
  lines.push_back(new Line(*P, *Q));
}

bool PolygonLine ::belongs(float x, float y) {
  bool belongs = false;
  unsigned long i = 0;
  while (i < lines.size() && !belongs) {
    belongs |= lines[i]->belongs(x, y);
    i++;
  }

  return belongs;
}
