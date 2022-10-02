#include "Point.cpp"
#include "../Utils.cpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

void CirclePoints(Point &P,int x, int y, int x_c, int y_c);

class Circle : public Primitive
{
    public:
        GLint Radius;
        Point *P_circle;
        Circle();
        Circle(Point &P, GLint R, GLFWwindow *currentWindow);
        ~Circle();

        /*overrides*/
        void draw() override;
        bool belongs(float x, float y) override;
        void translate(float x, float y) override;
        void scale(float k) override;
        void rotate(float theta) override;

        friend ordered_json &operator << (ordered_json &jprimitive, Circle &C);

    private:
        GLFWwindow *window;
};

Circle :: Circle()
{
      type = CIRCLE;
      Radius = 100;
      P_circle = nullptr;
}

Circle :: Circle(Point &P, GLint R, GLFWwindow *currentWindow)
{
      type = CIRCLE;
      window = currentWindow;
      P_circle = &P;
      Radius = R;
}

Circle :: ~Circle()
{
    delete P_circle;
}

void Circle ::draw()
{
    if (Radius > 1 && P_circle != nullptr)
    {
        window = P_circle->getWindow();
        Point P(0.0f,0.0f,window);
        P.R = P_circle->R;
        P.G = P_circle->G;
        P.B = P_circle->B;
        P.PointSize = P_circle->PointSize;
        P.setColor();
        P.setSize();

        /*Mid-Point Algorith*/
        GLint x = 0, y = Radius;
        GLint d = 1 - Radius;
        GLint x_c, y_c;
        x_c = P_circle->x;
        y_c = P_circle->y;

        CirclePoints(P, x, y, x_c, y_c);

        while (y > x)
        {
            if(d < 0) 
                d += (x << 1) + 3;
            else
            {
                d+= ((x - y) << 1) + 5;
                y--;
            }
            x++;
            CirclePoints(P, x, y, x_c, y_c);
        }
    }
}

void CirclePoints(Point &P,int x, int y, int x_c, int y_c)
{
    P.x = x + x_c;
    P.y = y + y_c;
    P.draw();
    P.x = y + x_c;
    P.y = x + y_c;
    P.draw();
    P.x = y + x_c;
    P.y = -x + y_c;
    P.draw();
    P.x = x + x_c;
    P.y = -y + y_c;
    P.draw();
    P.x = -x + x_c;
    P.y = -y + y_c;
    P.draw();
    P.x = -y + x_c;
    P.y = -x + y_c;
    P.draw();
    P.x = -y + x_c;
    P.y = x + y_c;
    P.draw();
    P.x = -x + x_c;
    P.y = y + y_c;
    P.draw();
}

bool Circle :: belongs(float x, float y)
{
    bool belongs = false;

    float difx,dify;
    difx = P_circle->x - x;
    dify = P_circle->y - y;
    if(std::abs(difx*difx + dify*dify - Radius * Radius) < 1600.0f)
        belongs = true;

    return belongs;
}

void Circle :: translate(float x, float y)
{
    P_circle->translate(x,y);
}

void Circle :: scale(float k)
{
    P_circle->scale(k);
    Radius *= std::abs(k);
}

void Circle :: rotate(float theta)
{
    P_circle->rotate(theta);
}

ordered_json &operator << (ordered_json &jprimitive, Circle &C)
{
    ordered_json obj;
	obj["ponto"]["x"] = C.P_circle->x / static_cast<float>(C.P_circle->width);
	obj["ponto"]["y"] = C.P_circle->y / static_cast<float>(C.P_circle->height);
    obj["raio"] = C.Radius / static_cast<float>(C.P_circle->width);
	obj["cor"]["r"] = static_cast<int>(C.P_circle->R * 250.0f);
	obj["cor"]["g"] = static_cast<int>(C.P_circle->G * 250.0f);
	obj["cor"]["b"] = static_cast<int>(C.P_circle->B * 250.0f);
	jprimitive.push_back(obj);

    return jprimitive;
}
