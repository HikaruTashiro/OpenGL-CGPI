#include <iostream>

class Pepega{
    public:
        int pog;
        double pepehands;
};

class PogChamp
{
    public:
        char c;
        float f;
};

int main()
{
    Pepega P;
    PogChamp T;

    P.pepehands = 69.0;
    T.c = '5';
    T.f = 42069.0f;

    void* p = &P;

    P.pog = 420;
    std::cout<<'\n'<<*(int*)p<<'\n';

    P.pog = 69;
    std::cout<<'\n'<<*(int*)p<<'\n';

    P.pog = 42069;
    std::cout<<'\n'<<*(int*)p<<'\n';
    
    p = &T;
    std::cout<<'\n'<<*(char*)p<<'\n';

    return 0;
}
