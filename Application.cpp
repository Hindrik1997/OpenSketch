//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include "Application.h"
#include "OpenGL/OpenGLRenderManager.h"
#include "GTK/gtkCallbacks.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Ellipse.h"
#include "Commands/AddRectangleCommand.h"
#include "Commands/AddEllipseCommand.h"

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

    m_file_box = gtk_vbox_new(0,0);

    m_acceptBttn = gtk_button_new_with_label("Accept");
    m_delete_shape = gtk_button_new_with_label("Delete shape");

    m_save_button = gtk_button_new_with_label("Save");
    m_load_button = gtk_button_new_with_label("Load");

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

    gtk_container_add(GTK_CONTAINER(m_file_box), m_save_button);
    gtk_container_add(GTK_CONTAINER(m_file_box), m_load_button);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_infoBox);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_acceptBttn);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_delete_shape);

    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_file_box);
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
    gtk_widget_show(m_load_button);
    gtk_widget_show(m_save_button);

    gtk_widget_show(m_file_box);

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

    //load button
    g_signal_connect(m_load_button, "clicked", G_CALLBACK(loadButton), NULL);

    //save button
    g_signal_connect(m_save_button, "clicked", G_CALLBACK(saveButton), NULL);



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

string trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

void split(const std::string &s, char delim, std::vector<string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Application::loadFromFile()
{
    const_cast<vector<Rectangle>&>(getGLManager().getRectangles()).clear();
    const_cast<vector<Ellipse>&>(getGLManager().getEllipses()).clear();

    std::vector<std::string> lines;

    std::ifstream input("datafile.txt");
    for(std::string line; getline(input, line);)
    {
        lines.push_back(line);
    }

    for(auto&& line : lines)
    {
        line = trim(line);
    }

    for(auto&& line : lines)
    {
        if(line.substr(0, 9) == std::string("rectangle"))
        {
            //add rect
            std::string part = line.substr(9, line.size() - 9);
            part = trim(part);

            std::vector<string> vals = split(part, ' ');
            execute(new AddRectangleCommand(std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2,std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2,std::atoi(vals[2].c_str()),std::atoi(vals[3].c_str())));
        }

        if(line.substr(0, 7) == std::string("ellipse"))
        {
            //add ellips
            std::string part = line.substr(7, line.size() - 7);
            part = trim(part);

            std::vector<string> vals = split(part, ' ');
            execute(new AddEllipseCommand(std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2,std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2,std::atoi(vals[2].c_str()),std::atoi(vals[3].c_str())));
        }
    }
    input.close();

    //history clearen
    while ( !m_history.empty() )
    {
        m_history.pop();
    }

    while(!m_redoHistory.empty())
    {
        m_redoHistory.pop();
    }

}

void Application::saveToFile()
{
    std::string lines("group ");

    int count = static_cast<int>(getGLManager().getEllipses().size() + getGLManager().getRectangles().size());

    lines.append(std::to_string(count));
    lines.append("\n");

    for(size_t i = 0; i < getGLManager().getRectangles().size(); ++i)
    {
        std::string line("  rectangle ");
        const Rectangle& rect = getGLManager().getRectangles()[i];

        line.append(std::to_string(static_cast<int>(rect.getPosition().x) - static_cast<int>(rect.getSize().x / 2)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getPosition().y) - static_cast<int>(rect.getSize().y / 2)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getSize().x)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getSize().y)));
        line.append("\n");
        lines.append(line);
    }
    for(size_t i = 0; i < getGLManager().getEllipses().size(); ++i)
    {
        std::string line("  ellipse ");
        const Ellipse& rect = getGLManager().getEllipses()[i];

        line.append(std::to_string(static_cast<int>(rect.getPosition().x) - static_cast<int>(rect.getSize().x / 2)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getPosition().y) - static_cast<int>(rect.getSize().y / 2)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getSize().x)));
        line.append(" ");
        line.append(std::to_string(static_cast<int>(rect.getSize().y)));
        line.append("\n");
        lines.append(line);
    }



    std::ofstream out("datafile2.txt");
    out << lines;
    out.close();

}
