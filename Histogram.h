#include "inc/GL/glew.h"
#include "inc/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Image.h"
#include <limits>
#define RGB_AMOUNT 256

typedef enum histogramtype {GREY = 0, RED = 0, BLUE, GREEN} HistogramType;

struct Histogram
{
    unsigned long plot[RGB_AMOUNT];

    Histogram();
    ~Histogram() = default;
    void clear(void);
    void build(Image* image, HistogramType type);
};

float plotHistogram(void* plot, int i);
