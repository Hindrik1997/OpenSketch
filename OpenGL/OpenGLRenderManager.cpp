//
// Created by Hindrik Stegenga on 26-9-16.
//

#include <cmath>
#include "OpenGLRenderManager.h"


void OpenGLRenderManager::render()
{
    //Clear color state setten
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz, deze gebruik ik niet in deze applicatie)
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_defaultShaderProgram);

    for(size_t i = 0; i < m_rectangles.size(); ++i)
    {
        GLint location = glGetUniformLocation(m_defaultShaderProgram, "transformMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_rectangles[i].getMatrix()));
        m_rectangles[i].getDrawObject().draw();
    }

    glUseProgram(0);

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_application.m_paintWindow);
}

OpenGLRenderManager::OpenGLRenderManager(Application& _app) : m_application(_app)
{
    createDefaultShaderProgram();

    addRectangle(300,300,300,300);

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

void OpenGLRenderManager::addRenderObject(vector<GLfloat> _verts, vector<GLint> _indices)
{
    size_t size = m_objects.capacity();
    size += 2;
    m_objects.reserve(size);
    m_objects.emplace_back(_verts, _indices);
}

void OpenGLRenderManager::addRectangle(int _posx, int _posy, int _width, int _height)
{
    //m_rectangles.push_back(Rectangle(*this,_posx, _posy, _width, _height));
    //space reserven
    size_t size = m_rectangles.capacity();
    size += 2;
    m_rectangles.reserve(size);
    m_rectangles.emplace_back(*this, _posx, _posy, _width, _height);
}