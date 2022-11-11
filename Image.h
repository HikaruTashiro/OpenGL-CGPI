#include "inc/GL/glew.h"
#include "imgui/imgui.h"
#include <iostream>
#include <string>
#pragma once

class Image
{
    int width;
    int height;
    std::string name;
    GLuint texture;
    unsigned char* image_data;
    int *access_row;

    public:
        Image(const char* filename);
        ~Image();
        inline ImTextureID getTexture(void); 
        inline ImVec2 getImageSize(void);
        inline const char* getName(void);
        inline int access_table(int j);
        inline unsigned char getRedAtPixel(int i, int j);
        inline unsigned char getBlueAtPixel(int i, int j);
        inline unsigned char getGreenAtPixel(int i, int j);
        void filterGreyScale(void);
        void filterBlackWhite(void);
        void filterLowPass(void);
        void filterHighPass(void);

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

inline int Image::access_table(int j)
{
    return access_row[j];
}

inline unsigned char Image::getRedAtPixel(int i ,int j)
{
    return image_data[(i << 2) + access_table(j)];
}

inline unsigned char Image::getBlueAtPixel(int i ,int j)
{
    return image_data[(i << 2) + 1 + access_table(j)];
}

inline unsigned char Image::getGreenAtPixel(int i ,int j)
{
    return image_data[(i << 2) + 2 + access_table(j)];
}
