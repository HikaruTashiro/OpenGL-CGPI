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
  void add(Point *P);
  void addFirst(Point *P, Point *Q);
  void draw() override;
  void translate(float x, float y) override;
  bool belongs(float x, float y) override;
  friend ordered_json &operator<<(ordered_json &jprimitive, PolygonLine &Pl);
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
  int n = lines.size();
  delete lines[n - 1]->P2;
  while (lines.size() > 0) {
    n = lines.size() - 1;
    lines[n]->P2 = nullptr;
    delete lines[n];
    lines.pop_back();
  }
}

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

void PolygonLine::translate(float x, float y) {
  for (Line *l : this->lines) {
    l->translate(x, y);
  }
}

ordered_json &operator<<(ordered_json &jprimitive, PolygonLine &Pl) {
  ordered_json obj, aux;
  unsigned long i;
  float width, height;
  width = static_cast<float>(Pl.lines[0]->P1->width);
  height = static_cast<float>(Pl.lines[0]->P1->height);

  for (i = 0; i < Pl.lines.size(); i++) {
    aux["x"] = Pl.lines[i]->P1->x / width;
    aux["y"] = Pl.lines[i]->P1->y / height;
    obj["ponto"].push_back(aux);
  }

  i--;
  obj["cor"]["r"] = static_cast<int>(Pl.lines[i]->P1->R * 250.0f);
  obj["cor"]["g"] = static_cast<int>(Pl.lines[i]->P1->G * 250.0f);
  obj["cor"]["b"] = static_cast<int>(Pl.lines[i]->P1->B * 250.0f);
  jprimitive.push_back(obj);
  return jprimitive;
}
