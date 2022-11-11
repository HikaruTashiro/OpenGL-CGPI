#include "inc/GL/glew.h"
#include "inc/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Image.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

bool fileExists(const char* filename);
void mainLoop(GLFWwindow* window);
void displayImage(Image *image, GLFWwindow* window);
void displayFilters(Image *image, GLFWwindow* window);
void upperMenu(Image* &image, bool &imageLoaded);

int main()
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

    /*ImGUI Initialization*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    /*Main loop of program*/
    mainLoop(window);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void mainLoop(GLFWwindow* window)
{
    /*Variables*/
    int display_w = 1920, display_h = 1080;
    bool imageLoaded = false;
    Image *image = nullptr;

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*Filter Window*/
        if(imageLoaded && image)
            displayFilters(image,window);

        /*Image Window*/
        if(imageLoaded && image)
            displayImage(image, window);

        upperMenu(image, imageLoaded);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

bool fileExists(const char* filename)
{
    std::ifstream f(filename);
    return f.good();
}

void displayImage(Image *image, GLFWwindow* window)
{
    ImGui::Begin(image->getName());
        ImGui::Image(image->getTexture(),image->getImageSize());
    ImGui::End();
}

void displayFilters(Image *image, GLFWwindow* window)
{
    ImGui::Begin("Image Filters");
    if(ImGui::Button("GreyScale"))
        image->filterGreyScale();
    ImGui::End();
}

void upperMenu(Image* &image, bool &imageLoaded)
{
    static char name[21];
    static bool button_confirm = false;
    static std::string filepath = "images/";
    ImGui::Begin("Menu");
    ImGui::InputText("ImageName", name, 20); 
    if(ImGui::Button("Load Image"))
    {
        button_confirm = true;
        filepath.append(name);
        filepath.append(".jpg");
    }

    if(button_confirm && (button_confirm = button_confirm && fileExists(filepath.c_str())))
    {
        if(!image)
        {
            image = new Image(filepath.c_str());
            imageLoaded = true;
            button_confirm = false;
            filepath.clear();
            filepath = "images/";
        }
        else
        {
            ImGui::Text("Confim overwrite?");
            if(ImGui::Button("No"))
            {
                button_confirm = false;
                filepath.clear();
                filepath = "images/";
            }
            ImGui::SameLine();
            if(ImGui::Button("Yes"))
            {
                delete image;
                image = new Image(filepath.c_str());
                button_confirm = false;
                imageLoaded = true;
                filepath.clear();
                filepath = "images/";
            }
        }
    }
    ImGui::End();
}
