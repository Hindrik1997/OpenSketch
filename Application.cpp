//
// Created by Hindrik Stegenga on 24-9-16.
//

#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include "Application.h"
#include "OpenGL/ShapeRenderManager.h"
#include "GTK/gtkCallbacks.h"
#include "Shapes/Shape.h"
#include "Commands/AddShapeCommand.h"
#include "OpenGL/RectangleDrawer.h"
#include "OpenGL/EllipseDrawer.h"
#include "Visitors/WriteToFileVisitor.h"
#include "Visitors/SetSelectedVisitor.h"

using std::string;
using std::cout;
using std::endl;

//Singleton getInstance() implementation.
Application &Application::getInstance() {
    static Application instance;
    return instance;
}

Application::Application()
{
}

//Cleans up
Application::~Application() {
    //Paintwindow hoeft niet gedelete te worden. GLFW doet dit!
    //ToolWindow pointer gaat via de GTKManager class, deze beheert de windows zelf!
    // (.createWindow() retourneert dan ook n reference ipv pointer om dit aan te geven)
    //Alleen GTKManager en de shaperendermanager moeten opgeruimd worden, hiervoor heb ik n leuke macro gemaakt.
    SAFE_PNTR_DEL(m_gtkManager);
    SAFE_PNTR_DEL(m_renderManager);
    SAFE_PNTR_DEL(m_ft);
    SAFE_PNTR_DEL(m_fontface);

    //GLFW vereist speciale clean up
    glfwTerminate();
}

//Sets up GLFW
bool Application::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    return true;
}

//Sets up GLEW
bool Application::initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed!" << std::endl;
        return false;
    }
    return true;
}

//Sets up GTK
void Application::initGTK() {
    m_gtkManager = new GTKManager(m_startupArgs.m_argc, m_startupArgs.m_argv);
}

//Main initialization function
void Application::initialize() {
    bool status;

    m_ft = new FT_Library;
    //FreeType initten
    if(FT_Init_FreeType(m_ft))
    {
        throw("Initialization failed!");
    }

    //Font laden

    m_fontface = new FT_Face;
    if(FT_New_Face(*m_ft, "consola.ttf", 0, m_fontface))
    {
        throw("Initialization failed!");
    }

    //text size = 16
    FT_Set_Pixel_Sizes(*m_fontface, 0, 128);


    //GLFW initten
    status = initGLFW();
    if(!status)
        throw("Initialization failed!");

    //Paint window maken
    m_paintWindow = glfwCreateWindow(PAINT_WINDOW_SIZE_X, PAINT_WINDOW_SIZE_Y, "Open Sketch", nullptr, nullptr);
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
    m_toolWindow = &m_gtkManager->createWindow(TOOL_WINDOW_SIZE_X, TOOL_WINDOW_SIZE_Y, "Tools");
    m_toolWindow->present();

    //Doet alle toolwindow stuff
    initToolWindow();

    //RenderManager initializen
    m_renderManager = new ShapeRenderManager(*this);
    resetState();

    setupFontRendering();
}

void Application::setupFontRendering() {
    //byte alignment uitzetten voor openGL, (Standaard texture size is multiple van 4 bytes, maar mijnes zijn 1 byte)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    for(GLubyte c = 0; c < 128; ++c)
    {
        if(FT_Load_Char(*m_fontface, c, FT_LOAD_RENDER ))
        {
            std::cout << "Loading char failed!" << std::endl;
            continue;
        }

        //tex generaten
        GLuint tex;
        glGenTextures(1,&tex);
        glBindTexture(GL_TEXTURE_2D,tex);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                (*m_fontface)->glyph->bitmap.width,
                (*m_fontface)->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                (*m_fontface)->glyph->bitmap.buffer
        );

        //text wrapping correct instellen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character ch = {
                tex,
                glm::ivec2((*m_fontface)->glyph->bitmap.width, (*m_fontface)->glyph->bitmap.rows),
                glm::ivec2((*m_fontface)->glyph->bitmap_left, (*m_fontface)->glyph->bitmap_top),
                static_cast<GLuint>((*m_fontface)->glyph->advance.x)
        };
        m_chars.insert(std::pair<GLchar,Character>(c,ch));
    }
    //cleanup
    FT_Done_Face(*m_fontface);
    FT_Done_FreeType(*m_ft);



    //buffers opzetten
    glGenVertexArrays(1,&m_text_vao);
    glGenBuffers(1, &m_text_vbo);
    glBindVertexArray(m_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_renderManager->createCustomShaderProgam("vertex_text.glsl","fragment_text.glsl",m_text_shader);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(PAINT_WINDOW_SIZE_X), 0.0f, static_cast<GLfloat>(PAINT_WINDOW_SIZE_Y));
    ShapeRenderManager::setCustomShaderProgram(m_text_shader);
    glUniformMatrix4fv(glGetUniformLocation(m_text_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

//Application loop
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

//Returns current paint window size
void Application::getPaintWindowSize(int &_width, int &_height) {
    glfwGetFramebufferSize(m_paintWindow, &_width, &_height);
}

//returns the current mouse position
glm::vec2 Application::getPaintWindowCursorPos() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_paintWindow, &xpos, &ypos);
    return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

//processes mouse and keyboard events
void Application::processMouseAndShapes()
{
    if(m_state != nullptr)
    m_state->doAction(this);
}

//Initialized de tool window
void Application::initToolWindow() {

    //border with instellen zodat button niet tegen zijkant komt
    gtk_container_set_border_width(GTK_CONTAINER(m_toolWindow->getWidgetPointer()), 10);

    m_box = gtk_vbox_new(0, 0);

    m_topBox = gtk_vbox_new(0, 0);
    m_bottomBox = gtk_vbox_new(0, 0);

    //TOPBOX

    m_null_mode_button = gtk_button_new_with_label("No edit mode");
    m_select_and_move_button = gtk_button_new_with_label("Select and move");
    m_select_and_edit = gtk_button_new_with_label("Select and modify");

    m_new_rectangle = gtk_button_new_with_label("New rectangle");
    m_new_ellips = gtk_button_new_with_label("New ellips");

    m_select_for_group_button = gtk_button_new_with_label("Select for new group");

    m_undo_button = gtk_button_new_with_label("Undo");
    m_redo_button = gtk_button_new_with_label("Redo");

    gtk_container_add(GTK_CONTAINER(m_topBox), m_null_mode_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_move_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_and_edit);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_rectangle);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_new_ellips);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_select_for_group_button);

    gtk_container_add(GTK_CONTAINER(m_topBox), m_undo_button);
    gtk_container_add(GTK_CONTAINER(m_topBox), m_redo_button);

    //BOTTOMBOX

    m_file_box = gtk_vbox_new(0, 0);

    m_acceptBttn = gtk_button_new_with_label("Accept");
    m_delete_shape = gtk_button_new_with_label("Delete shape");

    m_group_button = gtk_button_new_with_label("Group shapes");
    m_ungroup_button = gtk_button_new_with_label("Ungroup shapes");

    m_save_button = gtk_button_new_with_label("Save");
    m_load_button = gtk_button_new_with_label("Load");

    m_infoBox = gtk_hbox_new(0, 0);

    m_leftColumn = gtk_vbox_new(0, 0);
    m_rightColumn = gtk_vbox_new(0, 0);

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

    gtk_container_add(GTK_CONTAINER(m_file_box), m_save_button);
    gtk_container_add(GTK_CONTAINER(m_file_box), m_load_button);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_infoBox);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_acceptBttn);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_delete_shape);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_group_button);
    gtk_container_add(GTK_CONTAINER(m_bottomBox), m_ungroup_button);

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
    gtk_widget_show(m_group_button);
    gtk_widget_show(m_ungroup_button);

    gtk_widget_show(m_file_box);

    gtk_widget_show(m_infoBox);

    gtk_widget_show(m_topBox);
    gtk_widget_show(m_bottomBox);
    gtk_widget_show(m_select_and_move_button);
    gtk_widget_show(m_select_and_edit);
    gtk_widget_show(m_null_mode_button);
    gtk_widget_show(m_new_rectangle);
    gtk_widget_show(m_new_ellips);
    gtk_widget_show(m_select_for_group_button);
    gtk_widget_show(m_undo_button);
    gtk_widget_show(m_redo_button);
    gtk_widget_show(m_box);

    //select mode button signal connecten
    g_signal_connect(m_select_and_move_button, "clicked", G_CALLBACK(selectmove), NULL);

    //no edit mode button connecten
    g_signal_connect(m_null_mode_button, "clicked", G_CALLBACK(noeditmode), NULL);

    //select edit mode button connecten
    g_signal_connect(m_select_and_edit, "clicked", G_CALLBACK(selectedit), NULL);

    //close button suppressen, want t paint window bepaalt t sluiten van t progamma.
    g_signal_connect(m_toolWindow->getWidgetPointer(), "delete_event", G_CALLBACK(suppress), NULL);

    //accept button
    g_signal_connect(m_acceptBttn, "clicked", G_CALLBACK(setEdited), NULL);

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

    //select for group button
    g_signal_connect(m_select_for_group_button, "clicked", G_CALLBACK(selectgroupButton), NULL);

    //group button
    g_signal_connect(m_group_button, "clicked", G_CALLBACK(groupButton), NULL);

    //ungroup button
    g_signal_connect(m_ungroup_button, "clicked", G_CALLBACK(ungroupButton), NULL);


}

void Application::renderText(string _text, GLfloat x, GLfloat y, GLfloat _scale, glm::vec3 _color) {

    //blenden enablen
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ShapeRenderManager::setCustomShaderProgram(m_text_shader);
    glUniform3f(glGetUniformLocation(m_text_shader, "textColor"), _color.x, _color.y, _color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_text_vao);

    //Over alle chars gaan
    std::string::const_iterator c;
    for (c = _text.begin(); c != _text.end(); c++)
    {
        Character ch = m_chars[*c];

        GLfloat xpos = x + ch.m_bearing.x * _scale;
        GLfloat ypos = y - (ch.m_size.y - ch.m_bearing.y) * _scale;

        GLfloat w = ch.m_size.x * _scale;
        GLfloat h = ch.m_size.y * _scale;
        //VBO updaten
        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
        };
        //Renderen van de glyph
        glBindTexture(GL_TEXTURE_2D, ch.m_texID);
        //update vbo geheugen
        glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //renderen
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.m_advance >> 6) * _scale; //bitshiften (2^6 is 64) om cursor te verplaatsen
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //blenden disablen
    glDisable(GL_BLEND);
}

//Resets current editing state
void Application::resetState() {

    SetSelectedVisitor v(false);
    for(auto&& s : m_selected_shapes)
    {
        m_renderManager->getShapes()[s]->accept(v);
    }

    m_selected_shapes.clear();
    setShapeDeleted(false);
    setShapeEdited(false);
    gtk_entry_set_text(GTK_ENTRY(m_posxBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_posyBox), "selected");
    gtk_entry_set_text(GTK_ENTRY(m_sizexBox), "Nothing");
    gtk_entry_set_text(GTK_ENTRY(m_sizeyBox), "selected");


    if(m_selectedShape != nullptr)
    {
        m_selectedShape->accept(v);
    }
    m_selectedShape = nullptr;
}

//string manipulation functions

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

//File read and save functions

bool interpret_line(int _index, vector<string>& _lines, Application* _context, int& _skipcounter, Group& _group)
{
    std::string line = _lines[_index];
    if(line.substr(0, 9) == std::string("rectangle"))
    {
        //add rect
        std::string part = line.substr(9, line.size() - 9);
        part = trim(part);

        std::vector<string> vals = split(part, ' ');
        unique_ptr<Shape> p = make_unique<Shape>(
                std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2,
                std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2,
                std::atoi(vals[2].c_str()),
                std::atoi(vals[3].c_str()),
                static_cast<Drawer*>(&RectangleDrawer::getInstance())
        );
        _group.getShapes().emplace_back(std::move(p));
        return true;
    }

    if(line.substr(0, 7) == std::string("ellipse"))
    {
        //add ellips
        std::string part = line.substr(7, line.size() - 7);
        part = trim(part);

        std::vector<string> vals = split(part, ' ');
        unique_ptr<Shape> p = make_unique<Shape>(
                std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2,
                std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2,
                std::atoi(vals[2].c_str()),
                std::atoi(vals[3].c_str()),
                static_cast<Drawer*>(&EllipseDrawer::getInstance())
        );
        _group.getShapes().emplace_back(std::move(p));
        return true;
    }

    if(line.substr(0,5) == std::string("group"))
    {
        //add group
        string t = line.substr(5, line.size());
        t = trim(t);
        int expectedcount = atoi(t.c_str());
        int linecount = 0;

        vector<unique_ptr<Shape>> temp;
        unique_ptr<Shape> g = std::make_unique<Group>(temp);
        Group* gr = dynamic_cast<Group*>(g.get());

        while(expectedcount != 0)
        {
            if(_index + linecount + 1 < _lines.size())
            {
                int c = 0;
                bool val = interpret_line(_index + linecount + 1, _lines, _context,c, *gr);
                if(val)
                    expectedcount--;
                linecount += c;
            }
            linecount++;
        }
        _skipcounter += linecount;
        _group.getShapes().emplace_back(std::move(g));
        return true;
    }
    return false;
}

void interpret_line(int _index, vector<string>& _lines, Application* _context, int& _skipcounter)
{
    string line = _lines[_index];
    if(line.substr(0, 9) == std::string("rectangle"))
    {
        //add rect
        std::string part = line.substr(9, line.size() - 9);
        part = trim(part);

        std::vector<string> vals = split(part, ' ');
        _context->execute(new AddShapeCommand(
                std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2,
                std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2,
                std::atoi(vals[2].c_str()),
                std::atoi(vals[3].c_str()),
                &RectangleDrawer::getInstance()
        ));
    }

    if(line.substr(0, 7) == std::string("ellipse"))
    {
        //add ellips
        std::string part = line.substr(7, line.size() - 7);
        part = trim(part);

        std::vector<string> vals = split(part, ' ');
        _context->execute(new AddShapeCommand(
                std::atoi(vals[0].c_str()) + std::atoi(vals[2].c_str()) / 2
                ,std::atoi(vals[1].c_str()) + std::atoi(vals[3].c_str()) / 2
                ,std::atoi(vals[2].c_str()),
                std::atoi(vals[3].c_str()),
                &EllipseDrawer::getInstance()
        ));
    }

    if(line.substr(0,5) == std::string("group"))
    {
        //add group
        string t = line.substr(5, line.size());
        t = trim(t);
        int expectedcount = atoi(t.c_str());

        vector<unique_ptr<Shape>> temp;
        unique_ptr<Shape> g = std::make_unique<Group>(temp);
        Group* gr = dynamic_cast<Group*>(g.get());

        int linecount = 0;
        while(expectedcount != 0)
        {
            if(_index + linecount + 1 < _lines.size())
            {
                int c = 0;
                bool val = interpret_line(_index + linecount + 1, _lines, _context,c, *gr);
                if(val)
                    expectedcount--;
                linecount += c;
            }
            linecount++;
        }
        _skipcounter += linecount;
        _context->getGLManager().getShapes().emplace_back(std::move(g));
    }
}

void Application::loadFromFile()
{
    getGLManager().getShapes().clear();

    std::vector<std::string> lines;

    std::ifstream input("load.txt");
    for(std::string line; getline(input, line);)
    {
        lines.push_back(line);
    }
    lines.erase(lines.begin()); //ignore first group

    for(auto&& line : lines)
    {
        line = trim(line);
    }

    for(int i = 0; i < lines.size(); ++i)
    {
        interpret_line(i, lines, this, i);
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

    int count = static_cast<int>(getGLManager().getShapes().size());

    lines.append(std::to_string(count));
    lines.append("\n");

    for(auto&& s : m_renderManager->getShapes())
    {
        WriteToFileVisitor v;
        s->accept(v);
        vector<string> t = v.getData();

        for(auto&& l : t)
        {
            l.insert(l.begin(), 1, '\t');
            l.append(1,'\n');
            lines.append(l);
        }
    }

    std::ofstream out("saved.txt");
    out << lines;
    out.close();
}

//accessor/getter functions

void Application::setShapeEdited(bool _val)
{
    m_isEdited = _val;
}

ShapeRenderManager& Application::getGLManager() {
    return *m_renderManager;
}

State *Application::getState() {
    return m_state;
}

void Application::setShapeDeleted(bool _val) {
    m_isDeleted = _val;
}

void Application::setState(State* _state) {
    resetState();
    m_state = _state;
}

vector<size_t>& Application::getSelectedShapes() {
    return m_selected_shapes;
}

bool Application::isM_isDegrouped() const {
    return m_isDegrouped;
}

void Application::setGroupDeformed(bool _val) {
    m_isDegrouped = _val;
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

Shape* &Application::getM_selectedShape() {
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
    return m_select_and_move_button;
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

FT_Library *Application::getM_ft() const {
    return m_ft;
}

FT_Face *Application::getM_fontface() const {
    return m_fontface;
}




