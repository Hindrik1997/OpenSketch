//
// Created by Hindrik Stegenga on 26-9-16.
//

#include "OpenGLRenderManager.h"
#include <string>
#include <ostream>
#include <fstream>
#include <streambuf>

using namespace std;


void OpenGLRenderManager::render()
{
    //Clear color state setten
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz, deze gebruik ik niet in deze applicatie)
    glClear(GL_COLOR_BUFFER_BIT);

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_Application.m_paintWindow);
}

OpenGLRenderManager::OpenGLRenderManager(Application& _app) : m_Application(_app)
{
    createDefaultShaders();
}

void OpenGLRenderManager::createDefaultShaders()
{
    const int debugBufSize = 524; //524 is groot genoeg voor error messages. gwn random groot genoeg nummer.
    GLint success;
    GLchar debugMsg[debugBufSize];


    //files inlezen
    ifstream vertex("vertex.glsl");
    ifstream fragment("fragment.glsl");

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
