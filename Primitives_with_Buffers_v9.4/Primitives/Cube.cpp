#include "Line.cpp"
#include "Point.cpp"
#include "Primitive.hpp"

class Cube : public Primitive
{
        Line* lines;
    public:
        Cube(float size, Point &P); 
        ~Cube();
        virtual void draw(void); 
        virtual bool belongs(float x, float y);
        virtual void translate(float x, float y);
        virtual void scale(float k);
        virtual void rotate(float theta);
};

Cube::Cube(float size, Point &P)
{
    GLFWwindow* window = P.getWindow();
    lines = new Line[12];
    Point *points = new Point[8];
    points[0].x = points[4].x = 0.0f;
    points[0].y = points[1].y = 0.0f;
    points[1].x = points[5].x = size;
    points[4].y = points[5].y = size;
    points[2].x = points[6].x = 1.70710678f * size;
    points[6].y = points[7].y = 1.70710678f * size;
    points[3].x = points[7].x = 0.70710678f * size;
    points[2].y = points[3].x = 0.70710678f * size;

    for (int i = 0; i < 4; i++)
    {
        lines[i].P1 = &points[i];
        lines[i].P2 = &points[(i + 1) == 4 ? 0 : i + 1];
        lines[i + 4].P1 = &points[i + 4 ];
        lines[i + 4].P2 = &points[(i + 1) == 4 ? 4 : i + 5];
        lines[i + 8].P1 = &points[i];
        lines[i + 8].P2 = &points[i + 4];
    }
}

Cube::~Cube()
{
    if(lines != nullptr)
        delete [] lines;
}

void Cube :: draw(void)
{
    
}

bool Cube :: belongs(float x, float y)
{
    /*Stub*/
    return false;
}

void Cube :: translate(float x, float y)
{
    /*Stub*/
}

void Cube :: scale(float k)
{
    /*Stub*/
}

void Cube :: rotate(float theta)
{
    /*Stub*/
}
