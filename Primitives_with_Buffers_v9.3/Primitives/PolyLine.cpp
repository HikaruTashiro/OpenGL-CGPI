#include "../Utils.cpp"
#include "./Line.cpp"
#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"
#include <cstdlib>
#include <new>
#include <utility>
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
  void scale(float r) override;
  void rotateItself(float deg) override;
  std::pair<float, float> getMidPoint();

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

void PolygonLine::scale(float r) {
  for (Line *l : this->lines) {
    l->scale(r);
  }
}

std::pair<float, float> PolygonLine::getMidPoint() {
  float midX = 0.0f;
  float midY = 0.0f;
  float count = 0.0f;
  for (int i = 0; i < this->lines.size(); i++) {
    // if (i == this->lines.size() - 1) {
    //  midX += this->lines[i]->P2->x;
    //  midY += this->lines[i]->P2->y;
    //} else {
    //  midX += this->lines[i]->P1->x;
    //  midY += this->lines[i]->P1->y;
    //  midX += this->lines[i]->P2->x;
    //  midY += this->lines[i]->P2->y;
    //}
    std::pair<float, float> temp = this->lines[i]->getMidPoint();
    midX += temp.first;
    midY += temp.second;

    count += 2.0f;
  }
  return std::make_pair(midX / (count), midY / (count));
}

void PolygonLine::rotateItself(float deg) {
  std::pair<float, float> midPoint = this->getMidPoint();
  std::pair<float, float> tempLineP1, tempLineP2;
  for (int i = 0; i < this->lines.size(); i++) {
    tempLineP1 = rotateAround(
        deg, std::make_pair(this->lines[i]->P1->x, this->lines[i]->P1->y),
        midPoint);
    this->lines[i]->P1->x = tempLineP1.first;
    this->lines[i]->P1->y = tempLineP1.second;
    if (i == this->lines.size() - 1) {
      tempLineP1 = rotateAround(
          deg, std::make_pair(this->lines[i]->P2->x, this->lines[i]->P2->y),
          midPoint);
      this->lines[i]->P2->x = tempLineP1.first;
      this->lines[i]->P2->y = tempLineP1.second;
    }
    // this->lines[i]->newPoints(tempLineP1, tempLineP2);
  }

  //  int size = this->lines.size() - 1;
  //  tempLineP1 = rotateAround(
  //      deg, std::make_pair(this->lines[size]->P1->x,
  //      this->lines[size]->P1->y), midPoint);
  //  this->lines[size]->P1->x = tempLineP1.first;
  //  this->lines[size]->P1->y = tempLineP1.second;
  // Point *last_p = this->lines[this->lines.size() - 1]->P2;
  // Point *first_p = this->lines[0]->P1;
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
