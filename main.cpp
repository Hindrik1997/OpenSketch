//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <iostream>



#include "GTKStuff/GTKManager.h"
#include "GTKStuff/GTKWindow.h"
#include "includeLibs.h"
#include "functions.h"

int main(int argc, char* argv[])
{
    GTKManager g(argc, argv);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenSketch", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GTKWindow& w = g.createWindow(1000, 1000, "SomeTITS");
    w.present();

    glViewport(0, 0, width, height);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        g.updateAPI();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}