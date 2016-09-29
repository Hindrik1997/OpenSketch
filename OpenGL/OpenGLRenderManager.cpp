//
// Created by Hindrik Stegenga on 26-9-16.
//

#include <cmath>
#include "OpenGLRenderManager.h"


void OpenGLRenderManager::render()
{
    //Clear color state setten
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz,
    // deze gebruik ik niet in deze applicatie)
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_defaultShaderProgram);

    for(size_t i = 0; i < m_rectangles.size(); ++i)
    {
        GLint transformLocation = glGetUniformLocation(m_defaultShaderProgram, "transformMatrix");
        GLint isSelLocation = glGetUniformLocation(m_defaultShaderProgram, "isSelected");

        GLint widthLocation = glGetUniformLocation(m_defaultShaderProgram, "width");
        GLint heightLocation = glGetUniformLocation(m_defaultShaderProgram, "height");

        glUniform1i(isSelLocation, m_rectangles[i].getSelected());
        glUniform1i(widthLocation, static_cast<GLint>(m_rectangles[i].getSize().x));
        glUniform1i(heightLocation, static_cast<GLint>(m_rectangles[i].getSize().y));

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(m_rectangles[i].getMatrix()));
        m_rectangles[i].getDrawObject().draw();
    }

    for(size_t i = 0; i < m_ellipses.size(); ++i)
    {
        GLint transformLocation = glGetUniformLocation(m_defaultShaderProgram, "transformMatrix");
        GLint isSelLocation = glGetUniformLocation(m_defaultShaderProgram, "isSelected");

        GLint widthLocation = glGetUniformLocation(m_defaultShaderProgram, "width");
        GLint heightLocation = glGetUniformLocation(m_defaultShaderProgram, "height");

        glUniform1i(isSelLocation, m_ellipses[i].getSelected());
        glUniform1i(widthLocation, static_cast<GLint>(m_ellipses[i].getSize().x));
        glUniform1i(heightLocation, static_cast<GLint>(m_ellipses[i].getSize().y));

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(m_ellipses[i].getMatrix()));
        m_ellipses[i].getDrawObject().draw();
    }

    glUseProgram(0);

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_application.m_paintWindow);
}

OpenGLRenderManager::OpenGLRenderManager(Application& _app) : m_application(_app)
{
    createDefaultShaderProgram();
}

void OpenGLRenderManager::createDefaultShaderProgram()
{
    createCustomShaderProgam(string("vertex.glsl"),string("fragment.glsl"), m_defaultShaderProgram);
}

void OpenGLRenderManager::setNullShaderProgram()
{
    glUseProgram(0);
}

void OpenGLRenderManager::setDefaultShaderProgram()
{
    glUseProgram(m_defaultShaderProgram);
}

void OpenGLRenderManager::setCustomShaderProgram(GLuint &_shaderProgam)
{
    glUseProgram(_shaderProgam);
}

void OpenGLRenderManager::createCustomShaderProgam(string _vertexShader, string _fragmentShader, GLuint& _shaderID)
{
    const int debugBufSize = 524; //524 is groot genoeg voor error messages. gwn random groot genoeg nummer.
    GLint success;
    GLchar debugMsg[debugBufSize];


    //files inlezen
    ifstream vertex(_vertexShader);
    ifstream fragment(_fragmentShader);

    string vSource((istreambuf_iterator<char>(vertex)), istreambuf_iterator<char>());
    string fSource((istreambuf_iterator<char>(fragment)), istreambuf_iterator<char>());

    const char* vp = vSource.c_str();
    const char* fp = fSource.c_str();

    //Shaders compilen

    GLuint vShader, fShader;
    vShader= glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vp, NULL);
    glShaderSource(fShader, 1, &fp, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);

    //Check of alles goed gelukt is

    //vertex shader
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vShader, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED COMPILATION VERTEXSHADER";
    }
    //fragment shader
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fShader, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED COMPILATION FRAGMENTSHADER";
    }

    //compilatie gelukt!
    //Shader program linken!
    _shaderID = glCreateProgram();
    glAttachShader(_shaderID, vShader);
    glAttachShader(_shaderID, fShader);
    glLinkProgram(_shaderID);

    //Checken of linken gelukt is!
    glGetProgramiv(_shaderID, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(_shaderID, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED LINKING SHADERPROGRAM";
    }

    //Rotzooi opruimen
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    //klaar!
}

void OpenGLRenderManager::addRectangle(int _posx, int _posy, int _width, int _height)
{
    m_rectangles.push_back(Rectangle(this, _posx, _posy, _width, _height));
}

Rectangle* OpenGLRenderManager::getSelectedRectangle() {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    for(size_t i = 0; i < m_rectangles.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_rectangles[i].getPosition().x - (m_rectangles[i].getSize().x / 2));
        xright = static_cast<int>(m_rectangles[i].getPosition().x + (m_rectangles[i].getSize().x / 2));
        ytop = static_cast<int>(m_rectangles[i].getPosition().y - (m_rectangles[i].getSize().y / 2));
        ybottom = static_cast<int>(m_rectangles[i].getPosition().y + (m_rectangles[i].getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
            return &m_rectangles[i];

    }
    return nullptr;
}

glm::vec2 OpenGLRenderManager::getMouseOffsetInRectangle(Rectangle& _rect,int _mousex, int _mousey) {
    int middleX, middleY;
    middleX = static_cast<int>(_rect.getPosition().x);
    middleY = static_cast<int>(_rect.getPosition().y);

    glm::vec2 result;

    if (middleX > _mousex)
        result.x = abs(middleX - _mousex);
    else
        result.x = -abs(middleX - _mousex);

    if(middleY > _mousey)
        result.y = abs(middleY - _mousey);
    else
        result.y = - abs(middleY - _mousey);
    return result;
}

Rectangle *OpenGLRenderManager::getSelectedRectanglePriority(Rectangle *_rect) {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    int indexSecond = -1;

    for(size_t i = 0; i < m_rectangles.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_rectangles[i].getPosition().x - (m_rectangles[i].getSize().x / 2));
        xright = static_cast<int>(m_rectangles[i].getPosition().x + (m_rectangles[i].getSize().x / 2));
        ytop = static_cast<int>(m_rectangles[i].getPosition().y - (m_rectangles[i].getSize().y / 2));
        ybottom = static_cast<int>(m_rectangles[i].getPosition().y + (m_rectangles[i].getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
        {
            if(&m_rectangles[i] == _rect)
                return &m_rectangles[i];
            else
                indexSecond = static_cast<int>(i);
        }
    }
    if(indexSecond != -1)
        return &m_rectangles[indexSecond];
    return nullptr;
}


void OpenGLRenderManager::addEllipse(int _posx, int _posy, int _width, int _height)
{
    m_ellipses.push_back(Ellipse(this, _posx, _posy, _width, _height));
}

Ellipse* OpenGLRenderManager::getSelectedEllipse() {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    for(size_t i = 0; i < m_ellipses.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_ellipses[i].getPosition().x - (m_ellipses[i].getSize().x / 2));
        xright = static_cast<int>(m_ellipses[i].getPosition().x + (m_ellipses[i].getSize().x / 2));
        ytop = static_cast<int>(m_ellipses[i].getPosition().y - (m_ellipses[i].getSize().y / 2));
        ybottom = static_cast<int>(m_ellipses[i].getPosition().y + (m_ellipses[i].getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
            return &m_ellipses[i];

    }
    return nullptr;
}

glm::vec2 OpenGLRenderManager::getMouseOffsetInEllipse(Ellipse& _ellipse,int _mousex, int _mousey) {
    int middleX, middleY;
    middleX = static_cast<int>(_ellipse.getPosition().x);
    middleY = static_cast<int>(_ellipse.getPosition().y);

    glm::vec2 result;

    if (middleX > _mousex)
        result.x = abs(middleX - _mousex);
    else
        result.x = -abs(middleX - _mousex);

    if(middleY > _mousey)
        result.y = abs(middleY - _mousey);
    else
        result.y = - abs(middleY - _mousey);
    return result;
}

Ellipse *OpenGLRenderManager::getSelectedEllipsePriority(Ellipse* _ellipse) {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    int indexSecond = -1;

    for(size_t i = 0; i < m_ellipses.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_ellipses[i].getPosition().x - (m_ellipses[i].getSize().x / 2));
        xright = static_cast<int>(m_ellipses[i].getPosition().x + (m_ellipses[i].getSize().x / 2));
        ytop = static_cast<int>(m_ellipses[i].getPosition().y - (m_ellipses[i].getSize().y / 2));
        ybottom = static_cast<int>(m_ellipses[i].getPosition().y + (m_ellipses[i].getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
        {
            if(&m_ellipses[i] == _ellipse)
                return &m_ellipses[i];
            else
                indexSecond = static_cast<int>(i);
        }
    }
    if(indexSecond != -1)
        return &m_ellipses[indexSecond];
    return nullptr;
}