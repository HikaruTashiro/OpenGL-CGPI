#include "Common.h"
#include <cstdlib>
#include <iostream>

void Error(const char* message)
{
    std::cout << message;
    exit(EXIT_FAILURE);
}

void Warning(const char* message)
{
    std::cout << message;
}
