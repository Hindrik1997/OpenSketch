//
// Created by Hindrik Stegenga on 26-9-16.
//

#include "OpenGLRenderManager.h"


void OpenGLRenderManager::render()
{
    //Clear color state setten
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz, deze gebruik ik niet in deze applicatie)
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_defaultShaderProgam);

    for(size_t i = 0; i < m_objects.size(); ++i)
    {
        m_objects[i].draw();
    }

    glUseProgram(0);

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_Application.m_paintWindow);
}

OpenGLRenderManager::OpenGLRenderManager(Application& _app) : m_Application(_app)
{
    createDefaultShaderProgram();


    addRenderObject(DrawableObject(vector<GLfloat>{
            0.5f,  0.5f, 0.0f,  // Top Right
            0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
            -0.5f,  0.5f, 0.0f   // Top Left
    }, vector<GLint>{
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
    }));
}

void OpenGLRenderManager::createDefaultShaderProgram()
{
    createCustomShaderProgam(string("vertex.glsl"),string("fragment.glsl"));
}

void OpenGLRenderManager::setNullShaderProgram()
{
    glUseProgram(0);
}

void OpenGLRenderManager::setDefaultShaderProgram()
{
    glUseProgram(m_defaultShaderProgam);
}

void OpenGLRenderManager::setCustomShaderProgram(GLuint &_shaderProgam)
{
    glUseProgram(_shaderProgam);
}

void OpenGLRenderManager::createCustomShaderProgam(string _vertexShader, string _fragmentShader)
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
    m_defaultShaderProgam = glCreateProgram();
    glAttachShader(m_defaultShaderProgam, vShader);
    glAttachShader(m_defaultShaderProgam, fShader);
    glLinkProgram(m_defaultShaderProgam);

    //Checken of linken gelukt is!
    glGetProgramiv(m_defaultShaderProgam, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(m_defaultShaderProgam, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED LINKING SHADERPROGRAM";
    }

    //Rotzooi opruimen
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    //klaar!
}

void OpenGLRenderManager::addRenderObject(DrawableObject&& _obj) {
    m_objects.push_back(move(_obj));
}
