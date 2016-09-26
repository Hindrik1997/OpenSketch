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
#include "GTKStuff/GTKManager.h"
#include "GTKStuff/GTKWindow.h"

struct Args
{
    int argc = 0; char** argv = nullptr;
};

class Application {
public:
    static Application& getInstance();
private:
    Application();
public:
    Application(const Application&) = delete;
    Application(const Application&&) = delete;
    void operator=(const Application&) = delete;
    void operator=(const Application&&) = delete;
    ~Application();
private:
    void initialize();
    bool initGLFW();
    bool initGLEW();
    void initGTK();
public:
    void run();
    void getPaintWindowSize(int& _width, int& _height);
    inline void setStartupArgs(Args _args);
    inline Args getStartupArgs();
private:
    GTKManager* m_gtkManager;
    GLFWwindow* m_paintWindow;
    GTKWindow* m_toolWindow;
    Args m_startupArgs;


};

inline void Application::setStartupArgs(Args _args) {
    m_startupArgs = _args;
}

inline Args Application::getStartupArgs() {
    return m_startupArgs;
}

#endif //OPEN_SKETCH_APPLICATION_H
