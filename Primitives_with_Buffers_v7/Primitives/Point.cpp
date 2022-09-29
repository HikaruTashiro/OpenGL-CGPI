#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../LStack.hpp"

#pragma once

class Point
{
	public:
        int type;
		GLint x , y;
		GLint PointSize;
		GLfloat R,G,B;
		Point(){PointSize = 2;x = y = 0; current_window = nullptr;}
		Point(GLint x, GLint y, GLFWwindow* window);
		void draw();
		GLFWwindow* getWindow(){return current_window;};

	private:
		GLFWwindow* current_window;
		GLint width,height;
		GLint FragmentShader;
};

Point :: Point(GLint x_axis, GLint y_axis, GLFWwindow* window)
{
	current_window = window;
	x = x_axis;
	y = y_axis;
	PointSize = 2;
	R = G = B = 1.0f;
	glfwGetWindowSize(window, &width, &height);
	width >>= 1;
	height >>= 1;
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	FragmentShader = glGetUniformLocation(program,"received_color");
}

void Point :: draw()
{
	GLuint vao[1];
	glUniform4f(FragmentShader,R,G,B,1.0f);
	glGenVertexArrays(1,vao);
	glBindVertexArray(vao[0]);

	GLfloat *vPositions = new GLfloat[2];
	vPositions[0] = x/(GLfloat)width;
	vPositions[1] = y/(GLfloat)height;
	glPointSize(PointSize);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) << 1, vPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	
	glDrawArrays(GL_POINTS,0,1);
	glDeleteVertexArrays(1,vao);
	delete []vPositions;
}
