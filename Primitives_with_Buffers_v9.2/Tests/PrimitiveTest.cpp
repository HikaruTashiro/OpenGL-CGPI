#include <type_traits>
#include <iostream>

class Primitive
{
    public:
        int type;
        Primitive(int n){type = n;}
        int getType()
        {
            return this->type;
        }
        virtual void draw(void) = 0;
        virtual bool belongs(void) = 0;
};

class Point : public Primitive
{
    public:
    Point(int n) : Primitive(n){}

    void draw(void) override;
    bool belongs(void) override;
};

class Line : public Primitive
{
    public:
    Line(int n) : Primitive(n){}
    void draw(void) override;
    bool belongs(void) override;
};

void Point :: draw()
{
    std::cout<<"This is the draw function of Point\n";
}

bool Point :: belongs()
{
    return true;
}

void Line :: draw()
{
    std::cout<<"This is the draw function of Line\n";
}

bool Line :: belongs()
{
    return false;
}

int main()
{
    Line *L = new Line(1);
    Point *P = new Point(2);
    Primitive* p = L;

    //std::cout<<"Line type = "<< *((int*)p + 2) << '\n'; 
    //std::cout<<"Point type = "<< *((int*)p + 2) << '\n'; 

    //if(*((int*)L + 2) == 1)
    //    L->draw();
    //if(*((int*)P + 2) == 2)
    //    P->draw();


    std::cout<<"Line type = "<< p->getType() << '\n'; 
    p = P;
    Point* Pog = dynamic_cast<Point*>(p);
    Pog = static_cast<Point*>(p);
    std::cout<<"Point type = "<< p->getType() << '\n'; 

    if(L->getType() == 1)
        L->draw();
    if(P->getType() == 2)
        P->draw();

    if(typeid(L) == typeid(Line*))
        std::cout<<"Name = "<< typeid(L).name() << '\n';

    return 0;
}
