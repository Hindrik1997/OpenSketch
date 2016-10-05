//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <algorithm>
#include <string>
#include <iostream>
#include "Application.h"
#include "OpenGL/OpenGLRenderManager.h"
#include "GTK/gtkCallbacks.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Ellipse.h"
#include "Commands/AddRectangleCommand.h"

using std::string;
using std::cout;
using std::endl;

Application &Application::getInstance() {
    static Application instance;
    return instance;
}
Application::Application()
{
}
Application::~Application() {
    //Paintwindow hoeft niet gedelete te worden. GLFW doet dit!
    //ToolWindow pointer gaat via de GTKManager class, deze beheert de windows zelf!
    // (.createWindow() retourneert dan ook n reference ipv pointer om dit aan te geven)
    //Alleen GTKManager en de rendermanager moeten opgeruimd worden, hiervoor heb ik n leuke macro gemaakt.
    SAFE_PNTR_DEL(m_gtkManager);
    SAFE_PNTR_DEL(m_renderManager);

    //GLFW vereist speciale clean up
    glfwTerminate();
}

bool Application::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
    m_paintWindow = glfwCreateWindow(1280, 800, "Open Sketch", nullptr, nullptr);
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
    m_toolWindow = &m_gtkManager->createWindow(300, 600, "Tools");
    m_toolWindow->present();

    //Doet alle toolwindow stuff
    initToolWindow();

    //RenderManager initializen
    m_renderManager = new OpenGLRenderManager(*this);
    resetState();
}

void Application::run()
{
    while(!glfwWindowShouldClose(m_paintWindow))
    {
        //Update GTK Windows enz.
        m_gtkManager->updateAPI();
        //Update GFLW window events.
        glfwPollEvents();

        //mouse and shape stuff
        processMouseAndShapes();

        //Renderen
        m_renderManager->render();
    }
}

void Application::getPaintWindowSize(int &_width, int &_height) {
    glfwGetFramebufferSize(m_paintWindow, &_width, &_height);
}

glm::vec2 Application::getPaintWindowCursorPos() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_paintWindow, &xpos, &ypos);
    return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}
void Application::processMouseAndShapes()
{
    if(m_state != nullptr)
    m_state->doAction(this);
}
void Application::initToolWindow()
{
    //border with instellen zodat button niet tegen zijkant komt
    gtk_container_set_border_width(GTK_CONTAINER(m_toolWindow->getWidgetPointer()), 10);

    m_box = gtk_vbox_new(0,0);

    m_topBox = gtk_vbox_new(0,0);
    m_bottomBox = gtk_vbox_new(0,0);

    //TOPBOX

    m_null_mode_button = gtk_button_new_with_label("No edit mode");
    m_select_and_m_move_button = gtk_button_new_with_label("Select and move");
    m_select_and_edit = gtk_button_new_with_label("Select and modify");

    m_new_rectangle = gtk_button_new_with_label("New rectangle");
    m_new_ellips = gtk_button_new_with_label("New ellips");

    m_undo_button = gtk_button_new_with_label("Undo");
    m_redo_button = gtk_button_new_with_label("Redo");

    gtk_container_add(GTK_CONTAINER(m_topBox), m_null_mode_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_m_move_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_edit);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_rectangle);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_ellips);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_undo_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_redo_button);

    //BOTTOMBOX

    m_acceptBttn = gtk_button_new_with_label("Accept");
    m_delete_shape = gtk_button_new_with_label("Delete shape");

    m_infoBox = gtk_hbox_new(0,0);

    m_leftColumn = gtk_vbox_new(0,0);
    m_rightColumn = gtk_vbox_new(0,0);

    m_posxBox = gtk_entry_new();
    m_posyBox = gtk_entry_new();

    m_sizexBox = gtk_entry_new();
    m_sizeyBox = gtk_entry_new();

    m_labelposx = gtk_label_new("x-pos");
    m_labelposy = gtk_label_new("y-pos");

    m_labelsizex = gtk_label_new("width");
    m_labelsizey = gtk_label_new("height");

    gtk_container_add(GTK_CONTAINER(m_leftColumn), m_labelposx);
    gtk_container_add(GTK_CONTAINER(m_leftColumn), m_posxBox);

    gtk_container_add(GTK_CONTAINER(m_rightColumn), m_labelposy);
    gtk_container_add(GTK_CONTAINER(m_rightColumn), m_posyBox);

    gtk_container_add(GTK_CONTAINER(m_leftColumn), m_labelsizex);
    gtk_container_add(GTK_CONTAINER(m_leftColumn), m_sizexBox);

    gtk_container_add(GTK_CONTAINER(m_rightColumn), m_labelsizey);
    gtk_container_add(GTK_CONTAINER(m_rightColumn), m_sizeyBox);

    gtk_container_add(GTK_CONTAINER(m_infoBox), m_leftColumn);
    gtk_container_add(GTK_CONTAINER(m_infoBox), m_rightColumn);

    //END

    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_infoBox);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_acceptBttn);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_delete_shape);
    gtk_container_add(GTK_CONTAINER(m_box), m_topBox);
    gtk_container_add(GTK_CONTAINER(m_box), m_bottomBox);

    gtk_container_add(GTK_CONTAINER(m_toolWindow->getWidgetPointer()), m_box);

    gtk_widget_show(m_labelposx);
    gtk_widget_show(m_labelposy);
    gtk_widget_show(m_posxBox);
    gtk_widget_show(m_posyBox);

    gtk_widget_show(m_labelsizex);
    gtk_widget_show(m_labelsizey);
    gtk_widget_show(m_sizexBox);
    gtk_widget_show(m_sizeyBox);

    gtk_widget_show(m_leftColumn);
    gtk_widget_show(m_rightColumn);

    gtk_widget_show(m_acceptBttn);
    gtk_widget_show(m_delete_shape);

    gtk_widget_show(m_infoBox);

    gtk_widget_show(m_topBox);
    gtk_widget_show(m_bottomBox);
    gtk_widget_show(m_select_and_m_move_button);
    gtk_widget_show(m_select_and_edit);
    gtk_widget_show(m_null_mode_button);
    gtk_widget_show(m_new_rectangle);
    gtk_widget_show(m_new_ellips);
    gtk_widget_show(m_undo_button);
    gtk_widget_show(m_redo_button);
    gtk_widget_show(m_box);

    //select mode button signal connecten
    g_signal_connect(m_select_and_m_move_button, "clicked", G_CALLBACK(selectmove), NULL);

    //no edit mode button connecten
    g_signal_connect(m_null_mode_button, "clicked", G_CALLBACK(noeditmode), NULL);

    //select edit mode button connecten
    g_signal_connect(m_select_and_edit, "clicked", G_CALLBACK(selectedit), NULL);

    //close button suppressen, want t paint window bepaalt t sluiten van t progamma.
    g_signal_connect(m_toolWindow->getWidgetPointer(),"delete_event", G_CALLBACK(suppress), NULL);

    //accept button
    g_signal_connect(m_acceptBttn,"clicked", G_CALLBACK(setEdited), NULL);

    //add rectangle
    g_signal_connect(m_new_rectangle, "clicked", G_CALLBACK(addRect), NULL);

    //add ellips
    g_signal_connect(m_new_ellips, "clicked", G_CALLBACK(addEllips), NULL);

    //delete button
    g_signal_connect(m_delete_shape, "clicked", G_CALLBACK(deleteShape), NULL);

    //undo button
    g_signal_connect(m_undo_button, "clicked", G_CALLBACK(undoCommand), NULL);

    //redo button
    g_signal_connect(m_redo_button, "clicked", G_CALLBACK(redoCommand), NULL);

}

void Application::setShapeEdited(bool _val)
{
    m_isEdited = _val;
}

OpenGLRenderManager& Application::getGLManager() {
    return *m_renderManager;
}

void Application::setShapeDeleted(bool _val) {
    m_isDeleted = _val;
}

void Application::setState(State* _state) {
    m_state = _state;
}

State *Application::getState() {
    return m_state;
}

void Application::resetState() {
    setShapeDeleted(false);
    setShapeEdited(false);
    gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
    gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");

    if(m_selectedShape.m_shapePointer != nullptr)
    {
        if(m_selectedShape.m_shapeT == shapeType::RectangleType)
        {
            static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(false);
        } else
        {
            static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(false);
        }
    }
    m_selectedShape.m_shapePointer = nullptr;
    m_selectedShape.m_shapeT = shapeType::NullType;
}

GLFWwindow *Application::getM_paintWindow() const {
    return m_paintWindow;
}

bool Application::isM_isEdited() const {
    return m_isEdited;
}

bool Application::isM_isDeleted() const {
    return m_isDeleted;
}

shapeInfo &Application::getM_selectedShape() {
    return m_selectedShape;
}

GtkWidget *Application::getM_posxBox() const {
    return m_posxBox;
}

GtkWidget *Application::getM_posyBox() const {
    return m_posyBox;
}

GtkWidget *Application::getM_sizexBox() const {
    return m_sizexBox;
}

GtkWidget *Application::getM_sizeyBox() const {
    return m_sizeyBox;
}

GtkWidget *Application::getM_labelposx() const {
    return m_labelposx;
}

GtkWidget *Application::getM_labelposy() const {
    return m_labelposy;
}

GtkWidget *Application::getM_labelsizex() const {
    return m_labelsizex;
}

GtkWidget *Application::getM_labelsizey() const {
    return m_labelsizey;
}

GtkWidget *Application::getM_delete_shape() const {
    return m_delete_shape;
}

GtkWidget *Application::getM_box() const {
    return m_box;
}

GtkWidget *Application::getM_topBox() const {
    return m_topBox;
}

GtkWidget *Application::getM_bottomBox() const {
    return m_bottomBox;
}

GtkWidget *Application::getM_null_mode_button() const {
    return m_null_mode_button;
}

GtkWidget *Application::getM_select_and_m_move_button() const {
    return m_select_and_m_move_button;
}

GtkWidget *Application::getM_select_and_edit() const {
    return m_select_and_edit;
}

GtkWidget *Application::getM_new_rectangle() const {
    return m_new_rectangle;
}

GtkWidget *Application::getM_new_ellips() const {
    return m_new_ellips;
}

GtkWidget *Application::getM_acceptBttn() const {
    return m_acceptBttn;
}

GtkWidget *Application::getM_infoBox() const {
    return m_infoBox;
}

GtkWidget *Application::getM_leftColumn() const {
    return m_leftColumn;
}

GtkWidget *Application::getM_rightColumn() const {
    return m_rightColumn;
}
