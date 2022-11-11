#include "imgui/imgui.h"
#include "inc/GL/glew.h"
#include <climits>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include "Common.h"
#define CHANNEL4 4

Image::Image(const char* filename)
{
    image_data = stbi_load(filename, &width, &height, NULL, 4);  
    name = filename;
    access_row = new int[height];
    access_row[0] = 0;
    for (int i = 1; i < height; i++)
        access_row[i] = access_row[i - 1] + width * CHANNEL4;
    
    if(image_data == NULL)
    {
        std::cout << stbi_failure_reason();
        Error("ERROR! (In : Image Constructor) -> could not create image, the file specified does not exists or is corrupted\n");
    }

    // Create a OpenGL texture identifier
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
    this->loadTexture();
}

Image::~Image()
{
    stbi_image_free(image_data);
    glDeleteTextures(1, &texture);
}

void Image::loadTexture()
{
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
}

void Image::filterGreyScale()
{
    int image_size = width * height * CHANNEL4;
    for (unsigned char* p = image_data; p != image_data + image_size ; p += CHANNEL4)
    {
        *p = (uint8_t) ((*(p) + *(p + 1) + *(p + 2)) / 3.0);
        *(p + 1) = *p;
        *(p + 2) = *p;
    }
    this->loadTexture();    
}

void Image::filterBlackWhite()
{

}

void Image::filterLowPass()
{

}

void Image::filterHighPass()
{

}

