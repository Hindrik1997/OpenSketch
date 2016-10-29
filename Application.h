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

/*
 * CRTP : Curriously Recurring Template Pattern
 * Door de derived classes zichzelf als template parameter mee te geven kan ik de commands toegang geven tot
 * members van de derived classes op een mooie manier, en meteen ook type-safety te garanderen.
 */

/*
 * COMMAND PATTERN
 * Doormiddel van het CRTP pattern hierboven, heb ik een variant van het command pattern geimplementeerd.
 * Er kan nu domweg gezegd worden: Application::getInstance().execute(new CommandSomething()) om een command toe te voegen.
 * Daarnaast kan je de Application::undo() en redo() functies gebruiken om te re/undo'en.
 */

#include "SupportClasses.h"
#include <iostream>
#include "includeLibs.h"
#include "GTK/GTKManager.h"
#include "GTK/GTKWindow.h"
#include "States/State.h"
#include "Commands/CommandContext.h"
#include "Shapes/Shape.h"


//prototypes
class OpenGLRenderManager;

class Application : public CommandContext<Application> {
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
    void initToolWindow();
public:
    //State pattern stuff
    void setState(State* _state);
    State* getState();
    void resetState();

    void loadFromFile();
    void saveToFile();

    //Init functions
    void initialize();
    void run();
    void getPaintWindowSize(int& _width, int& _height);
    inline void setStartupArgs(Args _args);
    inline Args getStartupArgs();
    void setShapeEdited(bool _val);
    void setShapeDeleted(bool _val);
    glm::vec2 getPaintWindowCursorPos() const;
    OpenGLRenderManager& getGLManager();
private:
    GTKManager* m_gtkManager;
    GLFWwindow* m_paintWindow;
    GTKWindow* m_toolWindow;
    OpenGLRenderManager* m_renderManager;
    Args m_startupArgs;

    //State stuf
    State* m_state = nullptr;
public:
    GLFWwindow *getM_paintWindow() const;
    bool isM_isEdited() const;
    bool isM_isDeleted() const;
    vector<size_t>& getSelectedShapes();
    Shape* &getM_selectedShape();
    GtkWidget *getM_posxBox() const;
    GtkWidget *getM_posyBox() const;
    GtkWidget *getM_sizexBox() const;
    GtkWidget *getM_sizeyBox() const;
    GtkWidget *getM_labelposx() const;
    GtkWidget *getM_labelposy() const;
    GtkWidget *getM_labelsizex() const;
    GtkWidget *getM_labelsizey() const;
    GtkWidget *getM_delete_shape() const;
    GtkWidget *getM_box() const;
    GtkWidget *getM_topBox() const;
    GtkWidget *getM_bottomBox() const;
    GtkWidget *getM_null_mode_button() const;
    GtkWidget *getM_select_and_m_move_button() const;
    GtkWidget *getM_select_and_edit() const;
    GtkWidget *getM_new_rectangle() const;
    GtkWidget *getM_new_ellips() const;
    GtkWidget *getM_acceptBttn() const;
    GtkWidget *getM_infoBox() const;
    GtkWidget *getM_leftColumn() const;
    GtkWidget *getM_rightColumn() const;
private:
    bool m_isEdited = false;
    bool m_isDeleted = false;
    Shape* m_selectedShape = nullptr;
    //FreeType
    FT_Library* m_ft = nullptr;
    FT_Face* m_fontface = nullptr;

    vector<size_t> m_selected_shapes;

    //ToolWindow pointers
    GtkWidget* m_box;
    GtkWidget* m_topBox;
    GtkWidget* m_bottomBox;
    GtkWidget* m_null_mode_button;
    GtkWidget* m_select_and_move_button;
    GtkWidget* m_select_and_edit;
    GtkWidget* m_new_rectangle;
    GtkWidget* m_new_ellips;
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
    GtkWidget* m_delete_shape;
    GtkWidget* m_undo_button;
    GtkWidget* m_redo_button;
    GtkWidget* m_file_box;
    GtkWidget* m_select_for_group_button;
    GtkWidget* m_group_button;
    GtkWidget* m_ungroup_button;
    GtkWidget* m_load_button;
    GtkWidget* m_save_button;
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
