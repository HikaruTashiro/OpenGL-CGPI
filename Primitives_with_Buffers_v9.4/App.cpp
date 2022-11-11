/* 
* @file App.cpp
* @author Jonas Edward Tashiro, Rafael Melloni Chacon Arnon, Luan Lopes Barbosa de Almeida
* @brief
* @version 9.4
* @date 2022-10-02
*
* @copyright Copyright (c) 2022
*/

#include "Data_Structures/Node.cpp"
#include "GLSL/Shader.cpp"
#include "Primitives/Circle.cpp"
#include "Primitives/Line.cpp"
#include "Primitives/Primitive.hpp"
#include "Primitives/Triangle.cpp"
#include "Primitives/Rectangle.cpp"
#include "Primitives/Point.cpp"
#include "Primitives/Polygon.cpp"
#include "Primitives/PolyLine.cpp"
#include "Primitives/Cube.cpp"
#include "Data_Structures/LList.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "json/json.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

void display(LList<Primitive*> &Lprimitives);
void displayTemp(LList<Point*> &Ltemp);
int buttonsImGui(int mode);
void pushPolygon(LList<Primitive*> &Lprimitives,LList<Point*> &Ltemp,int mode);
Node<Primitive*>* selectPrimitives(LList<Primitive*> &Lprimitives,ImVec2 &mouse_clicked);
void clearTemp(LList<Point*> &Ltemp, bool destroy);
void clearPrimitive(LList<Primitive*> &Lprimitives);

/*JSON*/
void serializeButton(LList<Primitive*> &Lprimitives, const char* fileName);
void deserializationButton(LList<Primitive*> &Lprimitives, const char* fileName,GLFWwindow* window);
Point* retrievePoint(nlohmann::basic_json<> &a,const char* mode, int display_w, int display_h, GLFWwindow* window);

/*Transformation Requests*/
void processTranslationRequest(Primitive* temp_p, float x, float y);
void processScaleRequest(Primitive* temp_p, float k);
void processRotationRequest(Primitive* temp_p, float theta);

/*
*	  The main function will initiate the GLEW and GLFW libraries, also will
*   create the window that will be displayed to the user, the main loop for the
*   window is in the while that keeps executing the display function and ImGui
*   windows.
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
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only

    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Primitives", NULL, NULL);
    if(window == NULL)
    {
        std::cout<<"Failed to Initialize Window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /*Initialize GLEW Library*/
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /*Function for Vertical Syncronization*/
    glfwSwapInterval(1);
    Shader* program_shader = new Shader("GLSL/vertShader.glsl","GLSL/fragShader.glsl");
    program_shader->activateShader();

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
    ImVec2 mouse_pos;               // Vector that will receive the mouse position on screen
    ImVec2 wselected_location(1920.0f,0.0f);
    LList<Point*> List_temporary;   // A temporary storage for Points when building Primitives
    LList<Primitive*> List_Primitives;   // Storage for Primitives
    float size = 1;
    int mode = POINT;
    int count = 0;      // counts how many points where added to List_temporary
    int radius = 100;
    bool select_window = false;
    Node<Primitive*>* node_selected = nullptr;
    const char* Mode_str[8] = {"Point Mode","Circle Mode","Line Mode","Rectangle Mode","Triangle Mode",
                                "Polygon Mode","Polygonal Line Mode","Select Mode"};
    const char* Type_str[7] = {"Point","Circle","Line","Rectangle","Triangle","Polygon","Polygonal Line"};
    int display_w, display_h;
    bool transforms[3] = {false, false, false};
    Line Horizontal(*new Point(-960.0f,0.0f,window),*new Point(960.0f,0.0f,window)), 
         Vertical(*new Point(0.0f,-540.0f,window),*new Point(0.0f,540.0f,window));
        
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*Coordinate lines*/
        Vertical.P1->y = -(display_h >> 1);
        Vertical.P2->y = display_h >> 1;
        Horizontal.P1->x = -(display_w >> 1);
        Horizontal.P2->x = display_w >> 1;
        Vertical.draw();
        Horizontal.draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*Toolbar for chosing the primitive types*/
        {
            ImGui::Begin("Toolbar",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                /*Change in state machine*/
                {
                    static char sfile_name[20] = "\0";
                    static char lfile_name[20] = "\0";
                    int mode_aux = mode;
                    mode = buttonsImGui(mode);
                    serializeButton(List_Primitives, sfile_name);
                    ImGui::SameLine();
                    ImGui::InputText("File Name to Save", sfile_name, 15);
                    deserializationButton(List_Primitives, lfile_name,window);
                    ImGui::SameLine();
                    ImGui::InputText("File Name to Load", lfile_name, 15);
                    if(mode_aux != mode)    //if there are changes in states the following change as well
                    {
                        pushPolygon(List_Primitives, List_temporary, mode_aux);
                        clearTemp(List_temporary, count < 3);   //if count < 3 is true then delete else dont (since it was added to the List_Primitives)
                        select_window = false;
                        count = 0;
                    }
                }
            ImGui::TextColored(ImVec4(0.819f,0.0745f,0.0745f,1.0f),Mode_str[mode]);            
            if(mode == CIRCLE)
                ImGui::SliderInt("Radius Size", &radius , 50, 300);
            ImGui::SliderFloat("Pixel Size", &size , 1.0, 2.0);
            ImGui::ColorEdit3("Primitive Color",(float *)&point_color); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        /*Mouse Event Handler*/
        if(ImGui::IsMouseClicked(0))    //Waits for Left Click
        {
            mouse_pos = ImGui::GetMousePos();
            if((mouse_pos.x > 580.0f || mouse_pos.y > 230.0f) && !select_window)
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
                count++;

                switch (mode)
                {
                    case POINT:
                    case CIRCLE:
                        if(mode == CIRCLE)
                            List_Primitives.add(new Circle(*P,radius,window));
                        else
                            List_Primitives.add(P);
                        count = 0;
                        break;
                    case LINE:
                    case RECTANGLE:
                        if(count < 2)                        
                            List_temporary.add(P);
                        else
                        {
                            Node<Point*> *node = List_temporary.getEnd();
                            if(mode == LINE)
                                List_Primitives.add(new Line(*node->value,*P));
                            else
                                List_Primitives.add(new Rectangle(*node->value,*P));
                            clearTemp(List_temporary, false);
                            count = 0;
                        }
                        break;
                    case TRIANGLE:
                        if(count < 3)                        
                            List_temporary.add(P);
                        else
                        {
                            Node<Point*> *node1 = List_temporary.getEnd();
                            Node<Point*> *node2 = node1->prev;
                            List_Primitives.add(new Triangle(*node2->value,*node1->value,*P));
                            clearTemp(List_temporary,false);   //the false is to specify that the points will not be deleted
                            count = 0;
                        }
                        break;
                    case POLYGON:
                    case PLINE:
                        List_temporary.add(P);
                        break;
                    case SELECT:
                        node_selected = selectPrimitives(List_Primitives, mouse_pos);
                        select_window = true;
                        break;
                }
            }
        }

        /*Select Primitives Window*/
        if(select_window && node_selected != nullptr)
        {
            static float x = 0.0f,y = 0.0f;
            static float theta = 0.0f;
            static float k = 1.0f;

            ImGui::Begin("Select Primitives Window",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImVec4(0.3411f,0.7372f,1.0f,1.0f),Type_str[node_selected->value->getType()]);            
            if(ImGui::Checkbox("Translate", transforms))
                transforms[1] = transforms[2] = false;
            if(transforms[0])
            {
                ImGui::SliderFloat("x axis:", &x, -(display_w >> 1), display_w >> 1);
                ImGui::SliderFloat("y axis:", &y, -(display_h >> 1), display_h >> 1);
            }

            if(ImGui::Checkbox("Rotate", (transforms + 1)))
                transforms[0] = transforms[2] = false;
            if(transforms[1])
                ImGui::SliderFloat("theta:",&theta, 0.0f, 359.0f);

            if(ImGui::Checkbox("Scale", (transforms + 2)))
                transforms[0] = transforms[1] = false;
            if(transforms[2])
                ImGui::SliderFloat("k:",&k, -50.0f, 50.0f);

            if(ImGui::Button("Apply Transformation"))
            {
                if(transforms[0])
                    processTranslationRequest(node_selected->value, x, y);
                else if (transforms[1])
                    processRotationRequest(node_selected->value,theta * M_PI / 180.0f);
                else if (transforms[2])
                    processScaleRequest(node_selected->value, k);
            }

            if(ImGui::Button("Delete Button"))
            {
                delete List_Primitives.remove(node_selected);
                node_selected = nullptr;
                select_window = false;
            }
            wselected_location.x = display_w - ImGui::GetWindowWidth();
            ImGui::End();
            ImGui::SetWindowPos("Select Primitives Window",wselected_location);
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        display(List_Primitives);
        displayTemp(List_temporary);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    clearPrimitive(List_Primitives);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
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
void display(LList<Primitive*> &Lprimitives)
{
    Point*P; Circle*C; Line*L;
    Rectangle* R; Triangle*T;
    PolygonLine* Pline; Polygon* Pgon;
    Node<Primitive*> *node;
    node = Lprimitives.getStart();

    while (node != nullptr)
    {
        switch (node->value->getType())
        {
            case POINT:
                P = dynamic_cast<Point*>(node->value);
                P->setColor();
                P->setSize();
                P->draw();
                break;
            case CIRCLE:
                C = dynamic_cast<Circle*>(node->value);
                C->draw();
                break;
            case LINE:
                L = dynamic_cast<Line*>(node->value);
                L->draw();
                break;
            case RECTANGLE:
                R = dynamic_cast<Rectangle*>(node->value);
                R->draw();
                break;
            case TRIANGLE:
                T = dynamic_cast<Triangle*>(node->value);
                T->draw();
                break;
            case POLYGON:
                Pgon = dynamic_cast<Polygon*>(node->value);
                Pgon->draw();
                break;
            case PLINE:
                Pline = dynamic_cast<PolygonLine*>(node->value);
                Pline->draw();
                break;
            default:
                break;
        }
        node = node->next;
    }
}

void displayTemp(LList<Point*> &Ltemp)
{
    if(Ltemp.getSize() >= 2)
    {
        Line L; 
        Node<Point*>* naux = Ltemp.getStart();
        while (naux->next != nullptr)
        {
            L.P1 = naux->value; 
            L.P2 = naux->next->value; 
            L.draw();
            naux = naux->next;
        }
        L.P1 = L.P2 = nullptr;
    }
}

int buttonsImGui(int mode)
{
    /*Buttons for chosing the Primitives*/
    ImGui::Text("Choose the Primitives bellow:");
    if(ImGui::Button("Point"))
        mode = POINT;
    ImGui::SameLine();
    if(ImGui::Button("Line"))
        mode = LINE;
    ImGui::SameLine();
    if(ImGui::Button("Triangle"))
        mode = TRIANGLE;
    ImGui::SameLine();
    if(ImGui::Button("Rectangle"))
        mode = RECTANGLE;
    ImGui::SameLine();
    if(ImGui::Button("Circle"))
        mode = CIRCLE;
    ImGui::SameLine();
    if(ImGui::Button("Polygon"))
        mode = POLYGON;
    ImGui::SameLine();
    if(ImGui::Button("Polygonal Line"))
        mode = PLINE;
    ImGui::SameLine();
    if(ImGui::Button("Select"))
        mode = SELECT;

    return mode;
}

/*
 *  Input:
 *  Output:
 *  Description:
 * */
void pushPolygon(LList<Primitive*> &Lprimitives, LList<Point*> &Ltemp, int mode)
{
    if(Ltemp.getSize() >= 3)
    {
        if(mode == POLYGON)
        {
            Polygon* Pg = new Polygon();
            Pg->addFirst(Ltemp.getStart()->value, Ltemp.getStart()->next->value);
            Ltemp.removeStart();
            Ltemp.removeStart();
            while (Ltemp.getSize() > 0)
            {
                Pg->add(Ltemp.getStart()->value);                                
                Ltemp.removeStart();
            }        
            Pg->linkline();
            Lprimitives.add(Pg);
        }
        else if (mode == PLINE)
        {
            PolygonLine* Pl = new PolygonLine();
            Pl->addFirst(Ltemp.getStart()->value, Ltemp.getStart()->next->value);
            Ltemp.removeStart();
            Ltemp.removeStart();
            while (Ltemp.getSize() > 0)
            {
                Pl->add(Ltemp.getStart()->value);                                
                Ltemp.removeStart();
            }        
            Lprimitives.add(Pl);
        }
    }
}

Node<Primitive*>* selectPrimitives(LList<Primitive*> &Lprimitives,ImVec2 &mouse_clicked)
{
    Point*P; Circle*C; Line*L;
    Rectangle* R; Triangle*T;
    PolygonLine* Pline; Polygon* Pgon;
    Node<Primitive*>* node_aux = Lprimitives.getStart();
    Node<Primitive*>* node_selected = nullptr;
    bool selected = false;

    while(node_aux != nullptr && selected == false)
    {
        switch (node_aux->value->getType())
        {
            case POINT:
                P = dynamic_cast<Point*>(node_aux->value);
                if(P->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case CIRCLE:
                C = dynamic_cast<Circle*>(node_aux->value);
                if(C->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case LINE:
                L = dynamic_cast<Line*>(node_aux->value);
                if(L->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case TRIANGLE:
                T = dynamic_cast<Triangle*>(node_aux->value);
                if(T->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case RECTANGLE:
                R = dynamic_cast<Rectangle*>(node_aux->value);
                if(R->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case POLYGON:
                Pgon = dynamic_cast<Polygon*>(node_aux->value);
                if(Pgon->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
            case PLINE:
                Pline = dynamic_cast<PolygonLine*>(node_aux->value);
                if(Pline->belongs(mouse_clicked.x, mouse_clicked.y))
                {
                    node_selected = node_aux;
                    selected = true;
                }
                break;
        } 
        node_aux = node_aux->next;
    }

    return node_selected;
}

void clearTemp(LList<Point*> &Ltemp, bool destroy)
{
    Point*P;
    while (Ltemp.getSize() != 0)
    {
        P = Ltemp.removeEnd();
        if(destroy)
            delete P;
    }
}

void clearPrimitive(LList<Primitive*> &Lprimitives)
{
    while (Lprimitives.getSize() != 0)
        delete Lprimitives.removeEnd();
}

void serializeButton(LList<Primitive*> &Lprimitives, const char* fileName)
{
    if(ImGui::Button("Save") && fileName[0] != '\0')
    {
        std::string name = fileName;
        name.append(".json");
        std :: ofstream file(name);
        ordered_json obj;
        if(!file)
        {
            ImGui::TextColored(ImVec4(1.0f,0.25f,0.0f,1.0f), "Failed to Save File");
            return;
        }

        Node<Primitive*> *node_aux = Lprimitives.getStart();    
        while (node_aux != nullptr)
        {
            switch (node_aux->value->getType())
            {
                case POINT:
                    obj["figura"]["ponto"] << *dynamic_cast<Point*>(node_aux->value);
                    break;
                case CIRCLE:
                    obj["figura"]["circulo"] << *dynamic_cast<Circle*>(node_aux->value);
                    break;
                case LINE:
                    obj["figura"]["linha"] << *dynamic_cast<Line*>(node_aux->value);
                    break;
                case RECTANGLE:
                    obj["figura"]["retangulo"] << *dynamic_cast<Rectangle*>(node_aux->value);
                    break;
                case TRIANGLE:
                    obj["figura"]["triangulo"] << *dynamic_cast<Triangle*>(node_aux->value);
                    break;
                case PLINE:
                    obj["figura"]["linha poligonal"] << *dynamic_cast<PolygonLine*>(node_aux->value);
                    break;
                case POLYGON:
                    obj["figura"]["poligono"] << *dynamic_cast<Polygon*>(node_aux->value);
                    break;
            }
            node_aux = node_aux->next;
        }
        file << obj.dump(1);
        file.close();
    }
}


void processTranslationRequest(Primitive* temp_p, float x, float y)
{
    switch (temp_p->getType())
    {
        case POINT:
            dynamic_cast<Point*>(temp_p)->translate(x,y);
            break;
        case CIRCLE:
            dynamic_cast<Circle*>(temp_p)->translate(x,y);
            break;
        case LINE:
            dynamic_cast<Line*>(temp_p)->translate(x,y);
            break;
        case RECTANGLE:
            dynamic_cast<Rectangle*>(temp_p)->translate(x,y);
            break;
        case TRIANGLE:
            dynamic_cast<Triangle*>(temp_p)->translate(x,y);
            break;
        case PLINE:
            dynamic_cast<PolygonLine*>(temp_p)->translate(x,y);
            break;
        case POLYGON:
            dynamic_cast<Polygon*>(temp_p)->translate(x,y);
            break;
    }

}

void processScaleRequest(Primitive* temp_p, float k)
{
    switch (temp_p->getType())
    {
        case POINT:
            dynamic_cast<Point*>(temp_p)->scale(k);
            break;
        case CIRCLE:
            dynamic_cast<Circle*>(temp_p)->scale(k);
            break;
        case LINE:
            dynamic_cast<Line*>(temp_p)->scale(k);
            break;
        case RECTANGLE:
            dynamic_cast<Rectangle*>(temp_p)->scale(k);
            break;
        case TRIANGLE:
            dynamic_cast<Triangle*>(temp_p)->scale(k);
            break;
        case PLINE:
            dynamic_cast<PolygonLine*>(temp_p)->scale(k);
            break;
        case POLYGON:
            dynamic_cast<Polygon*>(temp_p)->scale(k);
            break;
    }

}

void processRotationRequest(Primitive* temp_p, float theta)
{
    switch (temp_p->getType())
    {
        case POINT:
            dynamic_cast<Point*>(temp_p)->rotate(theta);
            break;
        case CIRCLE:
            dynamic_cast<Circle*>(temp_p)->rotate(theta);
            break;
        case LINE:
            dynamic_cast<Line*>(temp_p)->rotate(theta);
            break;
        case RECTANGLE:
            dynamic_cast<Rectangle*>(temp_p)->rotate(theta);
            break;
        case TRIANGLE:
            dynamic_cast<Triangle*>(temp_p)->rotate(theta);
            break;
        case PLINE:
            dynamic_cast<PolygonLine*>(temp_p)->rotate(theta);
            break;
        case POLYGON:
            dynamic_cast<Polygon*>(temp_p)->rotate(theta);
            break;
    }
}

void deserializationButton(LList<Primitive*> &Lprimitives, const char* fileName, GLFWwindow* window)
{
    if(ImGui::Button("Load") && fileName[0] != '\0')
    {
        clearPrimitive(Lprimitives);
        int display_w, display_h;
        glfwGetWindowSize(window, &display_w, &display_h);
        std::string name = fileName;
        name.append(".json");
        std::ifstream file(name);
        if(!file)
        {
            ImGui::TextColored(ImVec4(1.0f,0.25f,0.0f,1.0f), "Failed to Open File");
            return;
        }
        json obj = json::parse(file);

        if (!obj["figura"]["ponto"].empty())
            for (auto& a : obj["figura"]["ponto"])
            {
                Point* P = retrievePoint(a,"p",display_w,display_h,window);
                Lprimitives.add(P);
            }
        if(!obj["figura"]["linha"].empty())
            for (auto& a : obj["figura"]["linha"])
            {
                Point* P = retrievePoint(a,"p1",display_w,display_h,window);
                Point* Q = retrievePoint(a,"p2",display_w,display_h,window);
                Lprimitives.add(new Line(*P,*Q));
            }
        if(!obj["figura"]["triangulo"].empty())
            for (auto& a : obj["figura"]["triangulo"])
            {
                Point* P = retrievePoint(a,"p1",display_w,display_h,window);
                Point* Q = retrievePoint(a,"p2",display_w,display_h,window);
                Point* R = retrievePoint(a,"p3",display_w,display_h,window);
                Lprimitives.add(new Triangle(*P,*Q,*R));
            }
        if(!obj["figura"]["retangulo"].empty())
            for (auto& a : obj["figura"]["retangulo"])
            {
                Point* P = retrievePoint(a,"p1",display_w,display_h,window);
                Point* Q = retrievePoint(a,"p2",display_w,display_h,window);
                Lprimitives.add(new Rectangle(*P,*Q));
            }
        if(!obj["figura"]["circulo"].empty())
            for (auto& a : obj["figura"]["circulo"])
            {
                Point* P = retrievePoint(a,"ponto",display_w,display_h,window);
                GLfloat radius = (display_w >> 1) * (GLfloat)a["raio"];
                Lprimitives.add(new Circle(*P,radius,window));
            }
        if(!obj["figura"]["linha poligonal"].empty())
            for (auto& a : obj["figura"]["linha poligonal"])
            {
                LList<Point*> Ltemp;
                GLfloat x, y;
                for (auto &p : a["ponto"])
                {
                    x = (display_w >> 1) * (GLfloat)p["x"];
                    y = (display_h >> 1) * (GLfloat)p["y"];
                    Point* P = new Point(x,y,window);
                    P->R = (GLfloat)a["cor"]["r"] / 256.0f;
                    P->G = (GLfloat)a["cor"]["g"] / 256.0f;
                    P->B = (GLfloat)a["cor"]["b"] / 256.0f;
                    Ltemp.add(P);                                    
                }
                pushPolygon(Lprimitives, Ltemp, PLINE);
            }
        if(!obj["figura"]["poligono"].empty())
            for (auto& a : obj["figura"]["poligono"])
            {
                LList<Point*> Ltemp;
                GLfloat x, y;
                for (auto &p : a["ponto"])
                {
                    x = (display_w >> 1) * (GLfloat)p["x"];
                    y = (display_h >> 1) * (GLfloat)p["y"];
                    Point* P = new Point(x,y,window);
                    P->R = (GLfloat)a["cor"]["r"] / 256.0f;
                    P->G = (GLfloat)a["cor"]["g"] / 256.0f;
                    P->B = (GLfloat)a["cor"]["b"] / 256.0f;
                    Ltemp.add(P);                                    
                }
                pushPolygon(Lprimitives, Ltemp, POLYGON);
            }

        file.close();    
    }
}

Point* retrievePoint(nlohmann::basic_json<> &a, const char* mode, int display_w, int display_h, GLFWwindow* window)
{
    GLfloat x, y;
    x = (display_w >> 1) * (GLfloat)a[mode]["x"];
    y = (display_h >> 1) * (GLfloat)a[mode]["y"];          
    //std::cout<<'('<<x<<','<<y<<')'<<'\n';
    Point* P = new Point(x,y,window);
    P->R = (GLfloat)a["cor"]["r"] / 256.0f;
    P->G = (GLfloat)a["cor"]["g"] / 256.0f;
    P->B = (GLfloat)a["cor"]["b"] / 256.0f;
    //std::cout<<'('<<P->R<<','<<P->G<<','<<P->B<<')'<<'\n';

    return P;
}
