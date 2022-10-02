#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>

class Shader
{
	GLuint shader;
	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError(void);
    std::string readShaderSource(const char *filePath);
    GLuint createShaderProgram(const char* vertShader, const char* fragShader);

    public:
        Shader() = default;
        Shader(const char* vertShader, const char* fragShader);
        void createProgram(const char* vertShader, const char* fragShader);
        void activateShader(void);
        void deleteShader(void);
};

Shader :: Shader(const char* vertShader, const char* fragShader)
{
    shader = createShaderProgram(vertShader,fragShader); 
}

void Shader :: activateShader(void)
{
    glUseProgram(shader);
}

void Shader :: deleteShader(void)
{
    glDeleteProgram(shader);
}

void Shader :: createProgram(const char* vertShader, const char* fragShader)
{
    shader = createShaderProgram(vertShader, fragShader);
}

/*
*	- Input: The name of a GLSL file containing a code.
*	- Output: A string containing the GLSL code.
*   - Description: reads the contents of a GLSL file and
*				creates a string containing the code.
*/
std::string Shader :: readShaderSource(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line = "";

    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

/*
*	- Input: two strings that refer to GLSL files. 
*	- Output: returns the address of compiled shader program.
*   - Description: reads GLSL Code for Vertex and Fragment Shaders,
*			 	then compiles both and procedes to link then into a
*               program that will be used in the Graphics Pipeline.
*/
GLuint Shader :: createShaderProgram(const char* vertShader, const char* fragShader)
{
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = readShaderSource(vertShader);
    std::string fragShaderStr = readShaderSource(fragShader);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    // catch errors while compiling shaders

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1)
    {
        std::cout << "vertex compilation failed";
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1)
    {
        std::cout << "fragment compilation failed";
        printShaderLog(fShader);
    }

    // catch errors while linking shaders

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();

    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) 
    {
        std::cout << "linking failed";
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void Shader :: printShaderLog(GLuint shader)
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

void Shader :: printProgramLog(int prog)
{
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

bool Shader :: checkOpenGLError()
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
