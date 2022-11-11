#include "inc/GL/glew.h"
#include "inc/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Image.hpp"
#include <cstdlib>
#include <iostream>

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

    /*Variables*/
    int display_w = 1920, display_h = 1080;

    int grey_image_width = 0;
    int grey_image_height = 0;
    int normal_image_width = 0;
    int normal_image_height = 0;
    GLuint grey_image_texture = 0;
    GLuint normal_image_texture = 0;
    Image img("RUSKI_BLYAT",NORMAL);
    //bool ret = LoadTextureFromFile("RUSKI_BLYAT.jpg", &normal_image_texture, &normal_image_width, &normal_image_height);
    //IM_ASSERT(ret);
    //bool retg = LoadTextureFromFileGrey("RUSKI_BLYAT.jpg", &grey_image_texture, &grey_image_width, &grey_image_height);
    //IM_ASSERT(retg);

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*Pepega Code*/

        ImGui::Begin("OpenGL Texture Text");
        ImGui::Text("2022:");
        ImGui::Text("size = %d x %d", normal_image_width, normal_image_height);
        ImGui::Image((void*)(intptr_t)normal_image_texture, ImVec2(normal_image_width, normal_image_height));

        ImGui::Text("1939:");
        ImGui::Text("size = %d x %d", grey_image_width, grey_image_height);
        ImGui::Image((void*)(intptr_t)grey_image_texture, ImVec2(grey_image_width, grey_image_height));
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}
