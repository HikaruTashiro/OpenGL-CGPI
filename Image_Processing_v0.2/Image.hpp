#include "inc/GL/glew.h"
#include "inc/GLFW/glfw3.h"
#include "imgui/imgui.h"

typedef enum mode{NORMAL, BLACK_WHITE} ImageMode;

class Image
{
    unsigned int with;
    unsigned int height;
    GLuint* texture;
    unsigned char* image_data;

    public:
        Image(const char* filename,ImageMode mode);
        ~Image();
        ImTextureID getTexture(void); 
        ImVec2 getImageSize(void);
};
