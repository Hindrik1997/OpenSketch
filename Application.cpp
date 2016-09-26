//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "Application.h"

Application &Application::getInstance() {
    static Application instance;
    return instance;
}

Application::Application() {
    initialize();
}

Application::~Application() {
    //Paintwindow hoeft niet gedelete te worden. GLFW doet dit!
    //ToolWindow pointer gaat via de GTKManager class, deze beheert de windows zelf!
    // (.createWindow() retourneert dan ook n reference ipv pointer om dit aan te geven)
    SAFE_PNTR_DEL(m_gtkManager);

    glfwTerminate();
}

bool Application::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    return true;
}

bool Application::initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed!" << std::endl;
        return false;
    }
    return true;
}

void Application::initGTK() {
    m_gtkManager = new GTKManager(m_startupArgs.argc, m_startupArgs.argv);
}

void Application::initialize() {
    bool status = true;
    status = initGLFW();
    if(!status)
        throw("Initialization failed!");

    //create paint window
    m_paintWindow = glfwCreateWindow(800, 600, "Open Sketch", nullptr, nullptr);
    if (m_paintWindow == nullptr)
    {
        std::cout << "Window creation failed!" << std::endl;
        glfwTerminate();
        throw("Initialization failed!");
    }
    glfwMakeContextCurrent(m_paintWindow);

    status = initGLEW();
    if(!status)
        throw("Initialization failed!");

    initGTK();

    //Set render viewport
    int width, height;
    getPaintWindowSize(width, height);

    glViewport(0, 0, width, height);

    //create tool window
    m_toolWindow = &m_gtkManager->createWindow(200, 300, "Tools");
    m_toolWindow->present();

}

void Application::run() {
    while(!glfwWindowShouldClose(m_paintWindow))
    {
        m_gtkManager->updateAPI();
        glfwPollEvents();

        //render loop here

        //present frame buffer by swapping
        glfwSwapBuffers(m_paintWindow);
    }
}

void Application::getPaintWindowSize(int &_width, int &_height) {
    glfwGetFramebufferSize(m_paintWindow, &_width, &_height);
}