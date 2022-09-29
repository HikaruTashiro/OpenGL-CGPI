/*
* @file App.cpp
* @author Jonas Edward Tashiro, Rafael Melloni Chacon Arnon, Luan Lopes Barbosa de Almeida
* @brief
* @version 0.5
* @date 2022-08-27
*
* @copyright Copyright (c) 2022
*/

#include "Primitives/Circle.cpp"
#include "Primitives/Line.cpp"
#include "Primitives/Triangle.cpp"
#include "Primitives/Rectangle.cpp"
#include "Primitives/Point.cpp"
#include "Primitives/Polygon.cpp"
#include "Primitives/PolyLine.cpp"
#include "Node.cpp"
#include "Utils.cpp"
#include "LStack.hpp"
#include "LList.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>

GLuint renderingProgram;
GLuint vbo[2];

std::string readShaderSource(const char *filePath);
GLuint createShaderProgram(const char* vertShader, const char* fragShader);
void init(GLFWwindow *window);
void display(GLFWwindow *window, double currentTime, LList<void*>* Lprimitives);
int buttonsImGui(LList<void*>* Lprimitives,LList<Point*>* Ltemp, int mode,int &count);
void pushPolygon(LList<void*>* Lprimitives, LList<Point*>*Ltemp,int mode);
inline int getType(Node<void*>* node)
{
    return *(int*) node->value;
}

void clearTemp(LList<Point*>* Ltemp)
{
    int points = Ltemp->getSize();
    if (points < 3)
        while (Ltemp->getSize() != 0)
            Ltemp->removeEnd();
}

/*
*	  The main function will initiate the GLEW and GLFW libraries, also will
*   create the window that will be displayed to the user, the main loop for the
*   window is in the while that keeps executing the display function.
*/
int main(void)
{
    /*Initializes GLFW Library*/
    if (!glfwInit())
      exit(EXIT_FAILURE);

    const char *glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only

    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Primitives", NULL, NULL);

    glfwMakeContextCurrent(window);

    /*Initialize GLEW Library*/
    if (glewInit() != GLEW_OK)
      exit(EXIT_FAILURE);

    /*Function for Vertical Syncronization*/
    glfwSwapInterval(1);
    init(window);

    /*ImGUI Initialization*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    /*Important Variables*/
    ImVec4 point_color = ImVec4(1.00f, 0.647f, 0.0f, 1.00f);
    ImVec2 mouse_pos;
    LList<Point*> List_temporary;
    LList<void*> List_Primitives;
    int size = 1;
    int mode = POINT;
    int count = 0;
    int radius = 100;
    const char* Mode_str[] = {"Point Mode","Circle Mode","Line Mode","Rectangle Mode","Triangle Mode",
                                "Polygon Mode","Polygonal Line Mode"};
    int display_w, display_h;
        
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*Toolbar for chosing the primitive types*/
        {
            ImGui::Begin("Toolbar",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            /*Mouse Event Handler*/
            if(ImGui::IsMouseClicked(0))    //Waits for Left Click
            {
                mouse_pos = ImGui::GetMousePos();
                if(mouse_pos.x > 500.0f || mouse_pos.y > 180.0f)
                {
                    mouse_pos.x -= display_w>>1; //Conversion to OpenGL coodinate system
                    mouse_pos.y -= display_h>>1;
                    mouse_pos.y = -mouse_pos.y;
                    //std::cout<<"mouse at x axis = "<<mouse_pos.x<<"\nmouse at y axis = "<<mouse_pos.y<<'\n';

                    Point* P = new Point(mouse_pos.x,mouse_pos.y,window);
                    P->R = point_color.x;
                    P->G = point_color.y;
                    P->B = point_color.z;
                    P->PointSize = size;
                    P->type = POINT;
                    count++;

                    if(mode == POINT)
                    {
                        List_Primitives.add(P);
                        count = 0;
                    }
                    else if(mode == CIRCLE)
                    {
                        List_Primitives.add(new Circle(P,radius,window));
                        count = 0;
                    }
                    else if(mode == LINE)
                    {
                        if(count < 2)                        
                            List_temporary.add(P);
                        else
                        {
                            Node<Point*> *node = List_temporary.getEnd();
                            List_Primitives.add(new Line(node->value,P));
                            List_temporary.removeEnd();
                            count = 0;
                        }
                    }
                    else if(mode == RECTANGLE)
                    {
                        if(count < 2)                        
                            List_temporary.add(P);
                        else
                        {
                            Node<Point*> *node = List_temporary.getEnd();
                            List_Primitives.add(new Rectangle(node->value,P));
                            List_temporary.removeEnd();
                            count = 0;
                        }
                    }
                    else if(mode == TRIANGLE)
                    {
                        if(count < 3)                        
                            List_temporary.add(P);
                        else
                        {
                            Node<Point*> *node1 = List_temporary.getEnd();
                            Node<Point*> *node2 = node1->prev;
                            List_Primitives.add(new Triangle(node2->value,node1->value,P));
                            List_temporary.removeEnd();
                            count = 0;
                        }
                    }
                    else
                        List_temporary.add(P);
                }
            }

            mode = buttonsImGui(&List_Primitives,&List_temporary,mode,count);

            //if(ImGui::Button("Debug"))
            //{
            //    Node<void*>* aux = List_Primitives.getStart();
            //    while (aux != nullptr) {
            //        std::cout<<'('<<((Point*)aux->value)->x<<','<<((Point*)aux->value)->y<<')'<<'\n';
            //        if(*(int*)aux->value == POINT)
            //            std::cout<<"POINT\n";

            //        aux = aux->next;                    
            //    }
            //}

            Node<void*>* aux = List_Primitives.getStart();
            while (aux != nullptr) {
                ((Point*)aux->value)->draw();
                aux = aux->next;                    
            }
            
            //buttonsImGui(mode, point_count,&List_Points,&List_Instruction); 

            ImGui::TextColored(ImVec4(0.819f,0.0745f,0.0745f,1.0f),Mode_str[mode]);            
            if(mode == CIRCLE)
                ImGui::SliderInt("Radius Size", &radius , 50, 300);

            ImGui::SliderInt("Pixel Size", &size , 1, 8);
            ImGui::ColorEdit3("clear color",(float *)&point_color); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        display(window, glfwGetTime(),&List_Primitives);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/*
*	- Input: The name of a GLSL file containing a code.
*	- Output: A string containing the GLSL code.
*   - Description: reads the contents of a GLSL file and
*				creates a string containing the code.
*/
std::string readShaderSource(const char *filePath)
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
GLuint createShaderProgram(const char* vertShader, const char* fragShader)
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
    Utils::checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1)
    {
        std::cout << "vertex compilation failed";
        Utils::printShaderLog(vShader);
    }

    glCompileShader(fShader);
    Utils::checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1)
    {
        std::cout << "fragment compilation failed";
        Utils::printShaderLog(fShader);
    }

    // catch errors while linking shaders

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    Utils::checkOpenGLError();

    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) 
    {
        std::cout << "linking failed";
        Utils::printProgramLog(vfProgram);
    }

    return vfProgram;
}

/*
*	- Input: receives the address GLFW window
*	- Output: none
*	- Description: Reads and compiles the Shader files, also creates the VBO
*               and activates it
*/
void init(GLFWwindow *window)
{
    renderingProgram = createShaderProgram("vertShader.glsl","fragShader.glsl");
    glGenBuffers(2,vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
}

/*
*	- Input: receives the address of the window that will display the
*           primitives and the variable curentTime that is used to 
*           control the frame of an animation, the other two are Stack_Points
*           one that keeps all points stored and other that instructs how to 
*           display them (if that point is going to create a circle or other
*           primitive).
*   - Output: none.
*   - Description: sets the color of the Background of the windown and 
*               displays the primitives inside the function.
*/
void display(GLFWwindow *window, double currentTime,LList<void*>* Lprimitives)
{
    glUseProgram(renderingProgram);
    Point*P; Circle*C; Line*L;
    Rectangle* R; Triangle*T;
    PolygonLine* Pline; Polygon* Pgon;
    Node<void*> *node;
    node = Lprimitives->getStart();

    for (int i = 0; i < Lprimitives->getSize(); i++)
    {
        switch (getType(node))
        {
            case POINT:
                P = (Point*) node->value;
                P->draw();
                break;
            case CIRCLE:
                C = (Circle*) node->value;
                C->draw();
                break;
            case LINE:
                L = (Line*) node->value;
                L->draw();
                break;
            case RECTANGLE:
                R = (Rectangle*) node->value;
                R->draw();
                break;
            case TRIANGLE:
                T = (Triangle*) node->value;
                T->draw();
                break;
            case POLYGON:
                Pgon = (Polygon*) node->value;
                Pgon->draw();
                break;
            case PLINE:
                Pline = (PolygonLine*) node->value;
                Pline->draw();
                break;
            default:
                break;
        }
        node = node->next;
    }
}

int buttonsImGui(LList<void*>* Lprimitives,LList<Point*>* Ltemp, int mode,int &count)
{
    /*Buttons for chosing the Primitives*/

    ImGui::Text("Choose the Primitives bellow:");
    if(ImGui::Button("Point"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = POINT;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Line"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = LINE;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Triangle"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = TRIANGLE;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Rectangle"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = RECTANGLE;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Circle"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = CIRCLE;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Polygon"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = POLYGON;
        clearTemp(Ltemp);
    }
    ImGui::SameLine();
    if(ImGui::Button("Polygonal Line"))
    {
        pushPolygon(Lprimitives, Ltemp, mode);
        count = 0, mode = PLINE;
        clearTemp(Ltemp);
    }

    return mode;
}

/*
 *  Input:
 *  Output:
 *  Description:
 * */
void pushPolygon(LList<void*>* Lprimitives, LList<Point*>*Ltemp, int mode)
{
    if(Ltemp->getSize() >= 3)
    {
        if(mode == POLYGON)
        {
            Polygon* Pg = new Polygon();
            Pg->addFirst(Ltemp->getStart()->value, Ltemp->getStart()->next->value);
            Ltemp->removeStart();
            Ltemp->removeStart();
            while (Ltemp->getSize() > 0)
            {
                Pg->add(Ltemp->getStart()->value);                                
                Ltemp->removeStart();
            }        
            Pg->linkline();
            Lprimitives->add(Pg);
        }
        else if (mode == PLINE)
        {
            PolygonLine* Pl = new PolygonLine();
            Pl->addFirst(Ltemp->getStart()->value, Ltemp->getStart()->next->value);
            Ltemp->removeStart();
            Ltemp->removeStart();
            while (Ltemp->getSize() > 0)
            {
                Pl->add(Ltemp->getStart()->value);                                
                Ltemp->removeStart();
            }        
            Lprimitives->add(Pl);
        }
    }
}
