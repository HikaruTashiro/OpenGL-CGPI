#include "Line.cpp"
#include "PolyLine.cpp"
#include <vector>

class Polygon : public PolygonLine {
public:
  Polygon();
  ~Polygon();
  void linkline();
};

Polygon::Polygon() : PolygonLine() { type = POLYGON; }

void Polygon ::linkline() {
  int top = lines.size();
  if (top > 2) {
    Line *L = new Line(*lines[top - 1]->P2, *lines[0]->P1);
    lines.push_back(L);
  }
}

Polygon::~Polygon() { lines.pop_back(); }
