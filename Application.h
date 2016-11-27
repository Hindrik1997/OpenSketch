//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_APPLICATION_H
#define OPEN_SKETCH_APPLICATION_H

#define PAINT_WINDOW_SIZE_X 1280
#define PAINT_WINDOW_SIZE_Y 800
#define TOOL_WINDOW_SIZE_X 300
#define TOOL_WINDOW_SIZE_Y 800


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
#include <map>

//prototypes
class ShapeRenderManager;

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
    void setupFontRendering();
public:
    void loadFromFile();
    void saveToFile();

    void renderText(string _text, GLfloat _x, GLfloat _y, GLfloat _scale, glm::vec3 _color);

    //Init functions
    void initialize();
    void run();

    inline void setStartupArgs(Args _args);
    inline Args getStartupArgs();

    glm::vec2 getPaintWindowCursorPos() const;
    ShapeRenderManager& getGLManager();
private:
    GTKManager* m_gtkManager;
    GLFWwindow* m_paintWindow;
    GTKWindow* m_toolWindow;
    ShapeRenderManager* m_renderManager;
    Args m_startupArgs;

    //FreeType
    FT_Library* m_ft = nullptr;
    FT_Face* m_fontface = nullptr;
    std::map<GLchar, Character> m_chars;
    GLuint m_text_vao;
    GLuint m_text_vbo;
    GLuint m_text_shader;

    //State stuf
    State* m_state = nullptr;
    bool m_isEdited = false;
    bool m_isDeleted = false;
    bool m_isDegrouped = false;
    Shape* m_selectedShape = nullptr;
    vector<size_t> m_selected_shapes;
public:
    void getPaintWindowSize(int& _width, int& _height);

    bool isM_isEdited() const;
    bool isM_isDeleted() const;
    bool isM_isDegrouped() const;

    void setShapeEdited(bool _val);
    void setShapeDeleted(bool _val);
    void setGroupDeformed(bool _val);

    vector<size_t>& getSelectedShapes();
    Shape* &getM_selectedShape();

    //State pattern stuff
    void setState(State* _state);
    State* getState();
    void resetState();


public:

    //GTK getters
    GLFWwindow *getM_paintWindow() const;
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
    GtkWidget *getM_ornament_field() const;

    //Freetype getters

    FT_Library *getM_ft() const;
    FT_Face *getM_fontface() const;
private:

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
    GtkWidget* m_ornament_field_label;
    GtkWidget* m_ornament_field;
    GtkWidget* m_add_top_ornament_button;
    GtkWidget* m_add_bottom_ornament_button;
    GtkWidget* m_add_left_ornament_button;
    GtkWidget* m_add_right_ornament_button;
};

inline void Application::setStartupArgs(Args _args) {
    m_startupArgs = _args;
}

inline Args Application::getStartupArgs() {
    return m_startupArgs;
}

#endif //OPEN_SKETCH_APPLICATION_H