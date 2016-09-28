//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_APPLICATION_H
#define OPEN_SKETCH_APPLICATION_H

/*
 * SINGLETON PATTERN
 * getInstance() retourneert de instance
 * de constructor is private, zodat je die niet zomaar kan callen
 * en de copy constructor, move constructor, assignment operator en move assignment operator zijn gedelete,
 * zodat ze niet gecalled kunnen worden.
 */

#include <iostream>
#include "includeLibs.h"
#include "GTK/GTKManager.h"
#include "GTK/GTKWindow.h"

//prototypes
class Rectangle;
class OpenGLRenderManager;

//Args struct
struct Args
{
    int m_argc = 0;
    char** m_argv = nullptr;
};

class Application {
public:
    static Application& getInstance();
private:
    Application();
public:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    ~Application();
private:
    bool initGLFW();
    bool initGLEW();
    void initGTK();
    void processMouseAndShapes();
public:
    void initialize();
    void run();
    void getPaintWindowSize(int& _width, int& _height);
    inline void setStartupArgs(Args _args);
    inline Args getStartupArgs();

    glm::vec2 getPaintWindowCursorPos() const;
private:
    GTKManager* m_gtkManager;
    GLFWwindow* m_paintWindow;
    GTKWindow* m_toolWindow;
    OpenGLRenderManager* m_renderManager;
    Args m_startupArgs;

    Rectangle* m_selectedLastFrame = nullptr;
    int m_xOffset = 0, m_yOffset = 0;

private:
    friend class OpenGLRenderManager;
};

inline void Application::setStartupArgs(Args _args) {
    m_startupArgs = _args;
}

inline Args Application::getStartupArgs() {
    return m_startupArgs;
}

#endif //OPEN_SKETCH_APPLICATION_H
