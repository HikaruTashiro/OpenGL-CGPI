#include <iostream>

float polynomial(float x, float *coefficients, int degree);

int main()
{
    float F[] = { -4.0f, 0.0f, 1.0f };
    int degree = 2;

    std::cout<<polynomial(4.0f, F, degree)<<'\n';

    float F1[] = { 1.0f, -8.0f, 9.0f, -5.0f, 3.0f };
    degree = 4;
    std::cout<<polynomial(2.0f, F1, degree)<<'\n';
}

float polynomial(float x, float *coefficients, int degree)
{
    int i = degree;
    float y = coefficients[i];
    //std::cout<<"At i = "<<i<<" ---> "<<y<<'\n';
    i--;

    while(i >= 0)
    {
        y = coefficients[i] + x * y;
        //std::cout<<"At i = "<<i<<" ---> "<<y<<'\n';
        i--;
    }
  
    return y;
}
