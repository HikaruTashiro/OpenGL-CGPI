#include "inc/GL/glew.h"
#include "imgui/imgui.h"
#include <string>

typedef enum mode{NORMAL, BLACK_WHITE} ImageMode;

class Image
{
    int width;
    int height;
    std::string name;
    GLuint texture;
    unsigned char* image_data;

    public:
        Image(const char* filename);
        ~Image();
        inline ImTextureID getTexture(void); 
        inline ImVec2 getImageSize(void);
        inline const char* getName(void);
        void filterGreyScale(void);

    private:
        void loadTexture(void);
};

/*Inline functions*/
inline ImTextureID Image::getTexture()
{
    return (void*)(intptr_t)texture;
}

inline ImVec2 Image::getImageSize()
{
    return ImVec2(width,height);
}

inline const char* Image::getName(void)
{
    return name.c_str();
}
