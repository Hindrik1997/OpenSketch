//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <string>
#include <iostream>
#include "Application.h"
#include "OpenGL/OpenGLRenderManager.h"
#include "GTK/gtkCallbacks.h"

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
    //Alleen GTKManager moet opgeruimd worden, hiervoor heb ik n leuke macro gemaakt.
    SAFE_PNTR_DEL(m_gtkManager);

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
    m_toolWindow = &m_gtkManager->createWindow(300, 400, "Tools");
    m_toolWindow->present();

    //Doet alle toolwindow stuff
    initToolWindow();

    //RenderManager initializen
    m_renderManager = new OpenGLRenderManager(*this);

}

void Application::setEditMode(EditMode _mode)
{
    m_editMode = _mode;
}
void Application::run() {

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
    switch(m_editMode)
    {
        case EditMode::Null_mode:
            setShapeEdited(false);
            gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
            gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");
            return;
        case EditMode::Select_and_move:
            selectMove();
            break;
        case EditMode::Select_and_edit:
            selectEdit();
            break;
    }
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

    gtk_container_add(GTK_CONTAINER(m_topBox), m_null_mode_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_m_move_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_edit);

    //BOTTOMBOX

    m_acceptBttn = gtk_button_new_with_label("Accept");

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


    gtk_widget_show(m_infoBox);

    gtk_widget_show(m_topBox);
    gtk_widget_show(m_bottomBox);
    gtk_widget_show(m_select_and_m_move_button);
    gtk_widget_show(m_select_and_edit);
    gtk_widget_show(m_null_mode_button);
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
}
void Application::selectMove()
{
    setShapeEdited(false);
    gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
    gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");



    int w,h;
    getPaintWindowSize(w,h);
    glm::vec2 v(static_cast<int>(getPaintWindowCursorPos().x),
                static_cast<int>(getPaintWindowCursorPos().y));
    if(v.x < 0)
        v.x = 0;
    if(v.x > w)
        v.x = w;
    if(v.y < 0)
        v.y = 0;
    if(v.y > h)
        v.y = h;

    static Rectangle* s_selectedRectangleLastFrame = nullptr;
    static int s_selectedRectxOffset = 0, s_selectedRectyOffset = 0;

    Rectangle* rect = nullptr;
    if(s_selectedRectangleLastFrame != nullptr)
        rect = m_renderManager->getSelectedRectanglePriority(s_selectedRectangleLastFrame);
    else
        rect = m_renderManager->getSelectedRectangle();

    if(rect != nullptr)
    {
        if(s_selectedRectangleLastFrame == nullptr)
        {
            //new triangle, no last triangle or not the same
            glm::vec2 offset;
            if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                offset = m_renderManager->getMouseOffsetInRectangle(*rect, static_cast<int>(v.x), static_cast<int>(v.y));
                rect->setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                s_selectedRectangleLastFrame = rect;
                s_selectedRectxOffset = static_cast<int>(offset.x);
                s_selectedRectyOffset = static_cast<int>(offset.y);
                rect->setSelected(true);
            }
            else
            {
                s_selectedRectxOffset = 0; s_selectedRectyOffset = 0; s_selectedRectangleLastFrame = nullptr;
            }
        }
        else
        {
            //same triangle
            if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                rect->setPosition(static_cast<int>(v.x + s_selectedRectxOffset),static_cast<int>(v.y + s_selectedRectyOffset));
            }
            else
            {
                if(s_selectedRectangleLastFrame != nullptr)
                    s_selectedRectangleLastFrame->setSelected(false);
                s_selectedRectxOffset = 0; s_selectedRectyOffset = 0; s_selectedRectangleLastFrame = nullptr;
            }
        }
    } else {
        if(s_selectedRectangleLastFrame != nullptr)
            s_selectedRectangleLastFrame->setSelected(false);
        s_selectedRectxOffset = 0; s_selectedRectyOffset = 0; s_selectedRectangleLastFrame = nullptr;
    }
}

void Application::selectEdit()
{
    int w,h;
    getPaintWindowSize(w,h);
    glm::vec2 v(static_cast<int>(getPaintWindowCursorPos().x),
                static_cast<int>(getPaintWindowCursorPos().y));
    if(v.x < 0)
        v.x = 0;
    if(v.x > w)
        v.x = w;
    if(v.y < 0)
        v.y = 0;
    if(v.y > h)
        v.y = h;


    static Rectangle* s_selectedRectangle = nullptr;

    Rectangle* currentSelected = m_renderManager->getSelectedRectangle();

    if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && currentSelected != nullptr && currentSelected != s_selectedRectangle)
    {
        //select shape

        //old deselecten
        if(s_selectedRectangle != nullptr)
            s_selectedRectangle->setSelected(false);

        s_selectedRectangle = currentSelected;
        s_selectedRectangle->setSelected(true);

        gtk_entry_set_text(GTK_ENTRY(m_posxBox), std::to_string(static_cast<int>(s_selectedRectangle->getPosition().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(m_posyBox), std::to_string(static_cast<int>(s_selectedRectangle->getPosition().y)).c_str());
        gtk_entry_set_text(GTK_ENTRY(m_sizexBox), std::to_string(static_cast<int>(s_selectedRectangle->getSize().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), std::to_string(static_cast<int>(s_selectedRectangle->getSize().y)).c_str());
    }

    if(s_selectedRectangle != nullptr)
    {
        s_selectedRectangle->setSelected(true);

        if(m_isEdited)
        {
            //get new info from tool and set

            string posx(gtk_entry_get_text(GTK_ENTRY(m_posxBox)));
            string posy(gtk_entry_get_text(GTK_ENTRY(m_posyBox)));
            string sizex(gtk_entry_get_text(GTK_ENTRY(m_sizexBox)));
            string sizey(gtk_entry_get_text(GTK_ENTRY(m_sizeyBox)));

            int px,py, sx,sy;

            try
            {
                //Throwed als de conversie faalt
                px = std::stoi(posx);
                py = std::stoi(posy);
                sx = std::stoi(sizex);
                sy = std::stoi(sizey);
            }
            catch (...)
            {
                //Niet accepten
                s_selectedRectangle->setSelected(false);
                s_selectedRectangle = nullptr;
                return;
            }

            s_selectedRectangle->setPosition(px,py);
            s_selectedRectangle->setSize(sx,sy);

            //shape stuff weer goed zetten
            s_selectedRectangle->setSelected(false);
            s_selectedRectangle = nullptr;

            //set edit to false
            setShapeEdited(false);

            gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
            gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");
        }
    }
    else
    {
        //nothing selected
        setShapeEdited(false);
        gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
        gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
        gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
        gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");
    }

}

void Application::setShapeEdited(bool _val)
{
    m_isEdited = _val;
}
