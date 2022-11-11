#include "Histogram.h"
#include "imgui/imgui.h"
#include <iostream>

Histogram::Histogram()
{
    for (int i = 0; i < RGB_AMOUNT; i++)
        plot[i] = 0;
}

void Histogram::clear()
{
    for (int i = 0; i < RGB_AMOUNT; i++)
        plot[i] = 0;
}

void Histogram::build(Image *image, HistogramType type)
{
    ImVec2 resolutiton = image->getImageSize();
    if(type == RED)
        for (int i = 0; i < resolutiton.x; i++)
            for (int j = 0; j < resolutiton.y; j++)
                plot[image->getRedAtPixel(i, j)]++;
    else if(type == BLUE)
        for (int i = 0; i < resolutiton.x; i++)
            for (int j = 0; j < resolutiton.y; j++)
                plot[image->getBlueAtPixel(i, j)]++;
    else
        for (int i = 0; i < resolutiton.x; i++)
            for (int j = 0; j < resolutiton.y; j++)
                plot[image->getGreenAtPixel(i, j)]++;
}

float plotHistogram(void* plot, int i)
{
    return *((unsigned long*)plot + i);
}
