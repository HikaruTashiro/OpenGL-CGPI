#include "imgui/imgui.h"
#include "inc/GL/glew.h"
#include <climits>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "Common.h"
#include "Image.h"
#include "stb_image.h"
#define CHANNEL4 4

Image::Image(const char *filename) {
  image_data = stbi_load(filename, &width, &height, NULL, 4);
  name = filename;
  access_row = new int[height];
  access_row[0] = 0;
  for (int i = 1; i < height; i++)
    access_row[i] = access_row[i - 1] + width * CHANNEL4;

  if (image_data == NULL) {
    std::cout << stbi_failure_reason();
    Error("ERROR! (In : Image Constructor) -> could not create image, the file "
          "specified does not exists or is corrupted\n");
  }

  // Create a OpenGL texture identifier
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE); // This is required on WebGL for non
                                     // power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
  this->loadTexture();
}

Image::~Image() {
  stbi_image_free(image_data);
  glDeleteTextures(1, &texture);
}

void Image::loadTexture() {
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
}

void Image::filterGreyScale() {
  int image_size = width * height * CHANNEL4;
  for (unsigned char *p = image_data; p != image_data + image_size;
       p += CHANNEL4) {
    *p = (uint8_t)((*(p) + *(p + 1) + *(p + 2)) / 3.0);
    *(p + 1) = *p;
    *(p + 2) = *p;
  }
  this->loadTexture();
}

void Image::filterLowPass() {
  int kernelRed, kernelGreen, kernelBlue;
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {
      kernelRed = getRedAtPixel(i, j + 1);
      kernelRed += getRedAtPixel(i, j - 1);
      kernelRed += getRedAtPixel(i + 1, j);
      kernelRed += getRedAtPixel(i - 1, j);
      kernelRed >>= 3;
      kernelGreen = getGreenAtPixel(i, j + 1);
      kernelGreen += getGreenAtPixel(i, j - 1);
      kernelGreen += getGreenAtPixel(i + 1, j);
      kernelGreen += getGreenAtPixel(i - 1, j);
      kernelGreen >>= 3;
      kernelBlue = getBlueAtPixel(i, j + 1);
      kernelBlue += getBlueAtPixel(i, j - 1);
      kernelBlue += getBlueAtPixel(i + 1, j);
      kernelBlue += getBlueAtPixel(i - 1, j);
      kernelBlue >>= 3;
      kernelRed += ((getRedAtPixel(i, j)) >> 1);
      kernelGreen += ((getGreenAtPixel(i, j)) >> 1);
      kernelBlue += ((getBlueAtPixel(i, j)) >> 1);
      setRedAtPixel(i, j, kernelRed);
      setGreenAtPixel(i, j, kernelGreen);
      setBlueAtPixel(i, j, kernelBlue);
    }
  }

  this->loadTexture();
}

void Image::filterBlackWhite() {
  int mean;
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {
      mean = getRedAtPixel(i, j);
      mean += getGreenAtPixel(i, j);
      mean += getBlueAtPixel(i, j);
      mean /= 3;
      if (mean > (255 / 2)) {
        setRedAtPixel(i, j, (char)0);
        setGreenAtPixel(i, j, (char)0);
        setBlueAtPixel(i, j, (char)0);
      } else {
        setRedAtPixel(i, j, (char)255);
        setGreenAtPixel(i, j, (char)255);
        setBlueAtPixel(i, j, (char)255);
      }
    }
  }

  this->loadTexture();
}

void Image::filterHighPass() {
  int kernelRed, kernelGreen, kernelBlue;
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {
      kernelRed = -1 * getRedAtPixel(i, j + 1);
      kernelRed -= getRedAtPixel(i, j - 1);
      kernelRed -= getRedAtPixel(i + 1, j);
      kernelRed -= getRedAtPixel(i - 1, j);
      kernelRed >>= 2;
      kernelGreen = -1 * getGreenAtPixel(i, j + 1);
      kernelGreen -= getGreenAtPixel(i, j - 1);
      kernelGreen -= getGreenAtPixel(i + 1, j);
      kernelGreen -= getGreenAtPixel(i - 1, j);
      kernelGreen >>= 2;
      kernelBlue = -1 * getBlueAtPixel(i, j + 1);
      kernelBlue -= getBlueAtPixel(i, j - 1);
      kernelBlue -= getBlueAtPixel(i + 1, j);
      kernelBlue -= getBlueAtPixel(i - 1, j);
      kernelBlue >>= 2;
      kernelRed += 2 * ((getRedAtPixel(i, j)));
      kernelGreen += 2 * ((getGreenAtPixel(i, j)));
      kernelBlue += 2 * ((getBlueAtPixel(i, j)));
      setRedAtPixel(i, j, kernelRed);
      setGreenAtPixel(i, j, kernelGreen);
      setBlueAtPixel(i, j, kernelBlue);
    }
  }
  this->loadTexture();
}
