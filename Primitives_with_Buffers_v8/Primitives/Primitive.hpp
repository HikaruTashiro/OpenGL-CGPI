/*
 * Abstract Class for Primitives
 * */
#ifndef _Primitive_hpp
#define _Primitive_hpp

#include <iostream>

class Primitive {
protected:
  int type;
  virtual void draw(void) = 0;
  virtual bool belongs(float x, float y) = 0;
  virtual void serializeToJSON(std::string fileName) = 0;

public:
  int getType() { return type; }
  virtual ~Primitive() = default;
};

#endif
