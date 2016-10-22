//
// Created by Hindrik Stegenga on 26-9-16.
//

#include <cmath>
#include "OpenGLRenderManager.h"
#include "../Shapes/Shape.h"


void OpenGLRenderManager::render()
{
    //Clear color state setten
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz,
    // deze gebruik ik niet in deze applicatie)
    glClear(GL_COLOR_BUFFER_BIT);

    m_rootGroup.draw();

    setNullShaderProgram();

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_application.m_paintWindow);
}

OpenGLRenderManager::OpenGLRenderManager(Application& _app) : m_application(_app), m_rootGroup(this)
{
}

void OpenGLRenderManager::setCustomShaderProgram(const GLuint& _shaderProgam)
{
    glUseProgram(_shaderProgam);
}

void OpenGLRenderManager::setNullShaderProgram()
{
    GLuint t = 0;
    setCustomShaderProgram(t);
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

Shape& OpenGLRenderManager::addShape(int _posx, int _posy, int _width, int _height, Drawer* _drawer)
{
    return m_rootGroup.addShape(this, _posx, _posy, _width, _height, _drawer);
}

Shape* OpenGLRenderManager::getSelectedShape() {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);
    return m_rootGroup.getSelectedShape(posx,posy);
}

glm::vec2 OpenGLRenderManager::getMouseOffsetInShape(Shape& _rect,int _mousex, int _mousey) {
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

vector<int> OpenGLRenderManager::getIndex(Shape *_shape) {
    return m_rootGroup.getIndex(_shape);
}

Shape *OpenGLRenderManager::getSelectedShapePriority(Shape* _shape) {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);
    bool b = false;
    return m_rootGroup.getSelectedShapePriority(posx,posy, _shape, b);
}
