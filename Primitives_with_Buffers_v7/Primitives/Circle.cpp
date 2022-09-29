#include "Point.cpp"
#include "../Utils.cpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

inline void CirclePoints(Point &P,int x, int y, int x_c, int y_c);

class Circle
{
    public:
        int type;
        GLint Radius;
        Point *P_circle;
        Circle()
        {
          Radius = 100;
          P_circle = nullptr;
        }
        Circle(Point *P, GLint R, GLFWwindow *currentWindow);
        void draw();

    private:
        GLFWwindow *window;
};

Circle :: Circle(Point *P, GLint R, GLFWwindow *currentWindow)
{
    type = CIRCLE;
    window = currentWindow;
    P_circle = P;
    Radius = R;
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

        /*
        //Pepega Algorith PepeHands
        GLfloat x, y;

        for (x = 0.0f; x <= Radius * 0.7071; x += P.PointSize)
        {
            y = std::sqrt((Radius + x) * (Radius - x));
            P.y = ( y + P_circle->y);
            P.x = ( x + P_circle->x);
            P.draw();
            P.y = (-y + P_circle->y);
            P.x = ( x + P_circle->x);
            P.draw();
            P.y = ( y + P_circle->y);
            P.x = (-x + P_circle->x);
            P.draw();
            P.y = (-y + P_circle->y);
            P.x = (-x + P_circle->x);
            P.draw();
        }

        for (y = 0.0f; y <= Radius * 0.7071; y += P.PointSize)
        {
            x = std::sqrt((Radius + y) * (Radius - y));
            P.y = ( y + P_circle->y);
            P.x = ( x + P_circle->x);
            P.draw();
            P.y = (-y + P_circle->y);
            P.x = ( x + P_circle->x);
            P.draw();
            P.y = ( y + P_circle->y);
            P.x = (-x + P_circle->x);
            P.draw();
            P.y = (-y + P_circle->y);
            P.x = (-x + P_circle->x);
            P.draw();
        }
        */
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

inline void CirclePoints(Point &P,int x, int y, int x_c, int y_c)
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
