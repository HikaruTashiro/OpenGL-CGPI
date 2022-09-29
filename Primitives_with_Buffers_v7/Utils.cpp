#include <new>
#include <iostream>
#include <GL/glew.h>

#pragma once

typedef enum{POINT, CIRCLE, LINE, RECTANGLE, TRIANGLE, POLYGON, PLINE} Instruction;

//ERROR Handling on GLSL
class Utils
{
	public:
		static void printShaderLog(GLuint shader)
		{
			int len = 0;
			int chWrittn = 0;
			char *log;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			if (len > 0) 
			{
				log = (char *)malloc(len);
				glGetShaderInfoLog(shader, len, &chWrittn, log);
				std::cout << "Shader Info Log: " << log;
				free(log);
			} 
		}

		static void printProgramLog(int prog) {
			int len = 0;
			int chWrittn = 0;
			char *log;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				log = (char *)malloc(len);
				glGetProgramInfoLog(prog, len, &chWrittn, log);
				std::cout << "Program Info Log: " << log;
				free(log);
			} 
		}

		static bool checkOpenGLError()
		{
			bool foundError = false;
			int glErr = glGetError();
			while (glErr != GL_NO_ERROR)
			{
				std::cout << "glError: " << glErr;
				foundError = true;
				glErr = glGetError();
			}
			return foundError;
		}
};
