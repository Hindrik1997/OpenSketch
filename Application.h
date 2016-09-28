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

enum class EditMode : char
{
    Null_mode,
    Select_and_move,
    Select_and_edit
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
    void selectMove();
    void initToolWindow();
    void selectEdit();
public:
    void initialize();
    void run();
    void getPaintWindowSize(int& _width, int& _height);
    inline void setStartupArgs(Args _args);
    inline Args getStartupArgs();
    void setEditMode(EditMode _mode);

    glm::vec2 getPaintWindowCursorPos() const;
private:
    GTKManager* m_gtkManager;
    GLFWwindow* m_paintWindow;
    GTKWindow* m_toolWindow;
    OpenGLRenderManager* m_renderManager;
    Args m_startupArgs;
    EditMode m_editMode = EditMode::Null_mode;
    //ToolWindow pointers
    GtkWidget* m_box;
    GtkWidget* m_topBox;
    GtkWidget* m_bottomBox;
    GtkWidget* m_null_mode_button;
    GtkWidget* m_select_and_m_move_button;
    GtkWidget* m_move_button;
    GtkWidget* m_acceptBttn;
    GtkWidget* m_infoBox;
    GtkWidget* m_leftColumn;
    GtkWidget* m_rightColumn;
    GtkWidget* m_posxBox;
    GtkWidget* m_posyBox;
    GtkWidget* m_sizexBox;
    GtkWidget* m_sizeyBox;
    GtkWidget* m_labelposx;
    GtkWidget* m_labelposy;
    GtkWidget* m_labelsizex;
    GtkWidget* m_labelsizey;
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
