//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <algorithm>
#include <string>
#include <iostream>
#include "Application.h"
#include "OpenGL/OpenGLRenderManager.h"
#include "GTK/gtkCallbacks.h"
#include "Rectangle.h"
#include "Ellipse.h"

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

    m_new_rectangle = gtk_button_new_with_label("New rectangle");
    m_new_ellips = gtk_button_new_with_label("New ellips");

    gtk_container_add(GTK_CONTAINER(m_topBox), m_null_mode_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_m_move_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_edit);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_rectangle);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_ellips);

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

}
void Application::selectMove()
{
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




    static shapeInfo s_selectedShapeLastFrame { nullptr, shapeType::NullType };
    static int s_selectedxOffset = 0, s_selectedyOffset = 0;



    shapeInfo selected;
    if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
    {
        if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
        {
            selected.m_shapePointer = m_renderManager->getSelectedRectanglePriority(static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer));
            selected.m_shapeT = shapeType::RectangleType;
        }
        else
        {
            selected.m_shapePointer = m_renderManager->getSelectedEllipsePriority(static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer));
            selected.m_shapeT = shapeType::EllipseType;
        }
    }
    else
    {
        selected.m_shapePointer = m_renderManager->getSelectedRectangle();
        selected.m_shapeT = shapeType::RectangleType;
        if(selected.m_shapePointer == nullptr)
        {
            selected.m_shapePointer = m_renderManager->getSelectedEllipse();
            if(selected.m_shapePointer != nullptr)
                selected.m_shapeT = shapeType::EllipseType;
        }
    }


    if(selected.m_shapePointer != nullptr)
    {
        if(s_selectedShapeLastFrame.m_shapePointer == nullptr)
        {
            //new triangle, no last triangle or not the same
            glm::vec2 offset;
            if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                if(selected.m_shapeT == shapeType::RectangleType)
                {
                    offset = m_renderManager->getMouseOffsetInRectangle(*static_cast<Rectangle*>(selected.m_shapePointer), static_cast<int>(v.x), static_cast<int>(v.y));
                    static_cast<Rectangle*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                    s_selectedShapeLastFrame.m_shapePointer = selected.m_shapePointer;
                    s_selectedShapeLastFrame.m_shapeT = selected.m_shapeT;
                    s_selectedxOffset = static_cast<int>(offset.x);
                    s_selectedyOffset = static_cast<int>(offset.y);
                    static_cast<Rectangle*>(selected.m_shapePointer)->setSelected(true);
                } else
                {
                    offset = m_renderManager->getMouseOffsetInEllipse(*static_cast<Ellipse*>(selected.m_shapePointer), static_cast<int>(v.x), static_cast<int>(v.y));
                    static_cast<Ellipse*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                    s_selectedShapeLastFrame.m_shapePointer = selected.m_shapePointer;
                    s_selectedShapeLastFrame.m_shapeT = selected.m_shapeT;
                    s_selectedxOffset = static_cast<int>(offset.x);
                    s_selectedyOffset = static_cast<int>(offset.y);
                    static_cast<Ellipse*>(selected.m_shapePointer)->setSelected(true);
                }
            }
            else
            {
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
            }
        }
        else
        {
            //same shape
            if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                if(selected.m_shapeT == shapeType::RectangleType)
                    static_cast<Rectangle*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + s_selectedxOffset),static_cast<int>(v.y + s_selectedyOffset));
                else
                    static_cast<Ellipse*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + s_selectedxOffset),static_cast<int>(v.y + s_selectedyOffset));
            }
            else
            {
                if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
                {
                    if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
                        static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);
                    else
                        static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);;
                }
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
            }
        }
    } else {
        if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
        {
            if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
                static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);
            else
                static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);

        }
        s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
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



    shapeInfo currentSelected;
    void* a = m_renderManager->getSelectedRectangle();
    if(a == nullptr)
    {
        a = m_renderManager->getSelectedEllipse();
        if(a != nullptr) {
            currentSelected.m_shapePointer = a;
            currentSelected.m_shapeT = shapeType::EllipseType;
        } else {
            currentSelected.m_shapePointer = nullptr;
            currentSelected.m_shapeT = shapeType::NullType;
        }
    } else
    {
        currentSelected.m_shapePointer = a;
        currentSelected.m_shapeT = shapeType::RectangleType;
    }



    if(glfwGetMouseButton(m_paintWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
       && currentSelected.m_shapePointer != nullptr
       && currentSelected.m_shapePointer != m_selectedShape.m_shapePointer)
    {
        //select shape

        //old deselecten
        if(m_selectedShape.m_shapePointer != nullptr) {

            if(m_selectedShape.m_shapeT == shapeType::RectangleType)
                static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(false);
            else
                static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(false);
        }

        m_selectedShape.m_shapePointer = currentSelected.m_shapePointer;
        m_selectedShape.m_shapeT = currentSelected.m_shapeT;

        if(m_selectedShape.m_shapeT == shapeType::RectangleType){
            static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(true);
            gtk_entry_set_text(GTK_ENTRY(m_posxBox), std::to_string(static_cast<int>(static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->getPosition().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_posyBox), std::to_string(static_cast<int>(static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->getPosition().y)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_sizexBox), std::to_string(static_cast<int>(static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->getSize().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), std::to_string(static_cast<int>(static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->getSize().y)).c_str());
        }
        else
        {
            static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(true);
            gtk_entry_set_text(GTK_ENTRY(m_posxBox), std::to_string(static_cast<int>(static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->getPosition().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_posyBox), std::to_string(static_cast<int>(static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->getPosition().y)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_sizexBox), std::to_string(static_cast<int>(static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->getSize().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), std::to_string(static_cast<int>(static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->getSize().y)).c_str());
        }
    }

    if(m_selectedShape.m_shapePointer != nullptr)
    {
        if(m_selectedShape.m_shapeT == shapeType::RectangleType){
            static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(true);
        } else
        {
            static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(true);
        }


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
                if(m_selectedShape.m_shapeT == shapeType::RectangleType){
                    static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(false);
                } else
                {
                    static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(false);
                }
                m_selectedShape.m_shapePointer = nullptr;
                m_selectedShape.m_shapeT = shapeType::NullType;
                return;
            }

            if(m_selectedShape.m_shapeT == shapeType::RectangleType){
                static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setPosition(px,py);
                static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSize(sx,sy);

                //shape stuff weer goed zetten
                static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(false);
            }
            else
            {
                static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setPosition(px,py);
                static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSize(sx,sy);

                //shape stuff weer goed zetten
                static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(false);
            }


            m_selectedShape.m_shapePointer = nullptr;
            m_selectedShape.m_shapeT = shapeType::NullType;
            //set edit to false
            setShapeEdited(false);

            gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
            gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");
        }

        if(m_isDeleted)
        {

            if(m_selectedShape.m_shapeT == shapeType::RectangleType){
                static_cast<Rectangle*>(m_selectedShape.m_shapePointer)->setSelected(false);
                vector<Rectangle>& vec = const_cast<vector<Rectangle>&>(m_renderManager->getRectangles());
                for( size_t i = 0; i < vec.size(); ++i)
                {
                    if(&vec[i] == m_selectedShape.m_shapePointer)
                    {
                        vec.erase(vec.begin() + i);
                        break;
                    }
                }
            } else
            {
                static_cast<Ellipse*>(m_selectedShape.m_shapePointer)->setSelected(false);
                vector<Ellipse>& vec = const_cast<vector<Ellipse>&>(m_renderManager->getEllipses());
                for( size_t i = 0; i < vec.size(); ++i)
                {
                    if(&vec[i] == m_selectedShape.m_shapePointer)
                    {
                        vec.erase(vec.begin() + i);
                        break;
                    }
                }
            }

            m_selectedShape.m_shapePointer = nullptr;
            m_selectedShape.m_shapeT = shapeType::NullType;
            setShapeEdited(false);
            setShapeDeleted(false);

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

OpenGLRenderManager& Application::getGLManager() {
    return *m_renderManager;
}

void Application::setShapeDeleted(bool _val) {
    m_isDeleted = _val;
}
