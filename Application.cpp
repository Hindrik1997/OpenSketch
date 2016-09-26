//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "Application.h"
#include "OpenGL/OpenGLRenderManager.h"

Application &Application::getInstance() {
    static Application instance;
    return instance;
}

Application::Application() {
}

Application::~Application() {
    //Paintwindow hoeft niet gedelete te worden. GLFW doet dit!
    //ToolWindow pointer gaat via de GTKManager class, deze beheert de windows zelf!
    // (.createWindow() retourneert dan ook n reference ipv pointer om dit aan te geven)
    //Alleen GTKManager moet opgeruimd worden, hiervoor heb ik n leuke macro gemaakt.
    SAFE_PNTR_DEL(m_gtkManager);

    //GLFW vereist speciale clean up
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
    m_gtkManager = new GTKManager(m_startupArgs.m_argc, m_startupArgs.m_argv);
}

void Application::initialize() {
    bool status = true;

    //GLFW initten
    status = initGLFW();
    if(!status)
        throw("Initialization failed!");

    //Paint window maken
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

    //GTK initten
    initGTK();

    //Viewport instellen
    int width, height;
    getPaintWindowSize(width, height);

    glViewport(0, 0, width, height);

    //Tools window maken
    m_toolWindow = &m_gtkManager->createWindow(200, 300, "Tools");
    m_toolWindow->present();

    //RenderManager initializen
    m_renderManager = new OpenGLRenderManager(*this);

}

void Application::run() {
    while(!glfwWindowShouldClose(m_paintWindow))
    {
        //Update GTK Windows enz.
        m_gtkManager->updateAPI();
        //Update GFLW window events.
        glfwPollEvents();
        //Renderen
        m_renderManager->render();
    }
}

void Application::getPaintWindowSize(int &_width, int &_height) {
    glfwGetFramebufferSize(m_paintWindow, &_width, &_height);
}