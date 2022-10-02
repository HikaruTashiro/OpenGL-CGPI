/*
 * Abstract Class for Primitives
 * */
#ifndef _Primitive_hpp
#define _Primitive_hpp
#include <ostream>
#include <string>

class Primitive
{
    protected:
        int type;
        virtual void draw(void) = 0; 
        virtual bool belongs(float x, float y) = 0;

    public:
        int getType() {return type;}
        virtual ~Primitive() = default;
        virtual void translate(float x, float y) = 0;
        virtual void scale(float k) = 0;
        virtual void rotate(float theta) = 0;
};

#endif 