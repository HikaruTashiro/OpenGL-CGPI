#include "../Utils.cpp"
#include "../json/json.hpp"
#include "Primitive.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <iostream>

#pragma once

class Point : public Primitive {
public:
  GLint x, y;
  GLfloat PointSize;
  GLfloat R, G, B;
  Point();
  Point(GLint x, GLint y, GLFWwindow *window);
  virtual ~Point();
  void serializeToJSON(std::string fileName) override;
  void draw(void) override;
  bool belongs(float x, float y) override;
  GLFWwindow *getWindow() { return current_window; };
  inline void setColor();
  inline void setSize();

private:
  GLFWwindow *current_window;
  GLint width, height;
  GLint FragmentShader;
  GLuint VAO[1];
  GLuint VBO[1];
};

/*Constructors*/
Point ::Point() {
  PointSize = 1.0f;
  x = y = 0;
  current_window = nullptr;
}

Point ::Point(GLint x_axis, GLint y_axis, GLFWwindow *window) {
  type = POINT;
  current_window = window;
  x = x_axis;
  y = y_axis;
  PointSize = 2;
  R = G = B = 1.0f;
  glfwGetWindowSize(window, &width, &height);
  width >>= 1;
  height >>= 1;
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  FragmentShader = glGetUniformLocation(program, "received_color");

  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glGenVertexArrays(1, VAO);
  glBindVertexArray(VAO[0]);
}

/*Destructor*/
Point::~Point() {
  glDeleteVertexArrays(1, VAO);
  glDeleteBuffers(1, VBO);
}

/*Implementation*/
void Point ::draw() {
  GLfloat *vPositions = new GLfloat[2];
  vPositions[0] = x / (GLfloat)width;
  vPositions[1] = y / (GLfloat)height;

  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) << 1, vPositions,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glDrawArrays(GL_POINTS, 0, 1);
  delete[] vPositions;
}

bool Point ::belongs(float x, float y) {
  return std::abs(this->x - x) < 20.0f && std::abs(this->y - y) < 20.0f;
}

void Point::serializeToJSON(std::string fileName) {
  std::ofstream file(fileName, std::ios::app);
  using nlohmann::json;
  json obj;
  obj["P"]["x"] = this->x;
  obj["P"]["y"] = this->y;
  obj["color"]["r"] = this->R;
  obj["color"]["g"] = this->G;
  obj["color"]["b"] = this->B;
  obj["type"] = this->type;
  std::cout << obj.dump() << "\n";
  file << obj.dump();
  file << "\n";
  file.close();
}

inline void Point ::setColor() { glUniform4f(FragmentShader, R, G, B, 1.0f); }

inline void Point ::setSize() { glPointSize(PointSize); }