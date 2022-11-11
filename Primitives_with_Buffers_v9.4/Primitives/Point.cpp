#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include "../json/json.hpp"
#include "Primitive.hpp"

#pragma once

class Point : public Primitive
{
	public:
		GLfloat x, y;
		GLfloat PointSize;
		GLfloat R,G,B;
		GLint width,height;
		GLFWwindow* current_window;
		Point();
		Point(GLint x, GLint y, GLFWwindow* window);
		~Point();
		/*override*/
		void draw(void) override;
		bool belongs(float x, float y) override;
		inline void translate(float x, float y) override;
		inline void scale(float k) override;
		inline void rotate(float theta) override;

		/*JSON*/
		friend std :: ostream &operator<<(std :: ostream &file, Point &P);
		friend ordered_json &operator<<(ordered_json &jprimitive,Point &P);
		inline GLFWwindow* getWindow(){return current_window;};
		inline void setColor();
		inline void setSize();

	private:
		GLint FragmentShader;
		GLuint VAO[1];
		GLuint VBO[1];
};

/*Constructors*/
Point :: Point()
{
	PointSize = 1.0f;
	x = y = 0;
	R = G = B = 0.25f;
	current_window = nullptr;
}

Point :: Point(GLint x_axis, GLint y_axis, GLFWwindow* window)
{
	type = POINT;
	current_window = window;
	x = x_axis;
	y = y_axis;
	PointSize = 1.0f;
	R = G = B = 0.5f;
	glfwGetWindowSize(window, &width, &height);
	width >>= 1;
	height >>= 1;
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	FragmentShader = glGetUniformLocation(program,"received_color");

    glGenBuffers(1,VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glGenVertexArrays(1,VAO);
	glBindVertexArray(VAO[0]);
}

/*Destructor*/
Point::~Point()
{
	glDeleteVertexArrays(1,VAO);
	glDeleteBuffers(1,VBO);
}

/*Implementation*/
void Point :: draw()
{
	GLfloat *vPositions = new GLfloat[2];
	vPositions[0] = x / static_cast<GLfloat>(width);
	vPositions[1] = y / static_cast<GLfloat>(height);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) << 1, vPositions, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	
	glDrawArrays(GL_POINTS,0,1);
	delete []vPositions;
}

bool Point :: belongs(float x, float y)
{
	return std::abs(this->x - x) < 20.0f && std::abs(this->y - y) < 20.0f ;
}

inline void Point::translate(float x, float y)
{
	this->x += x;
	this->y += y;
}

inline void Point::scale(float k)
{
	x *= k;
	y *= k;
}

inline void Point::rotate(float theta)
{
	float cos_theta, sin_theta, aux = x;	
	cos_theta = std::cos(theta);
	sin_theta = std::sin(theta);
	x = x * cos_theta - y * sin_theta;
	y = aux * sin_theta + y * cos_theta;

}

inline void Point :: setColor()
{
	glUniform4f(FragmentShader,R,G,B,1.0f);
}

inline void Point :: setSize()
{
	glPointSize(PointSize);
}

ordered_json &operator<<(ordered_json &jprimitive,Point &P)
{
	ordered_json obj;
	obj["p"]["x"] = P.x / static_cast<float>(P.width);
	obj["p"]["y"] = P.y / static_cast<float>(P.height);
	obj["cor"]["r"] = (int) (P.R * 256.0f);
	obj["cor"]["g"] = (int) (P.G * 256.0f);
	obj["cor"]["b"] = (int) (P.B * 256.0f);
	jprimitive.push_back(obj);

	return jprimitive;
}

std :: ostream& operator<<(std :: ostream &file, Point &P)
{
	json obj;
	obj["ponto"]["p"]["x"] = P.x;
	obj["ponto"]["p"]["y"] = P.y;
	obj["ponto"]["cor"]["r"] = static_cast<int>(P.R * 250.0f);
	obj["ponto"]["cor"]["g"] = static_cast<int>(P.G * 250.0f);
	obj["ponto"]["cor"]["b"] = static_cast<int>(P.B * 250.0f);

	file << obj.dump(1,'\t');
	return file;
}
