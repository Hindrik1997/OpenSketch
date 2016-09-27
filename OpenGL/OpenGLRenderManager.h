//
// Created by Hindrik Stegenga on 26-9-16.
//

#ifndef OPEN_SKETCH_OPENGLRENDERER_H
#define OPEN_SKETCH_OPENGLRENDERER_H

#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <streambuf>
#include "DrawableObject.h"

using namespace std;

#include "../includeLibs.h"
#include "../Application.h"

class OpenGLRenderManager {
private:
    Application& m_Application;
    GLuint m_defaultShaderProgram;
    vector<DrawableObject> m_objects;
public:
    OpenGLRenderManager(Application& _app);

    OpenGLRenderManager(const OpenGLRenderManager&) = delete;
    OpenGLRenderManager(const OpenGLRenderManager&&) = delete;
    void operator=(const OpenGLRenderManager&) = delete;
    void operator=(const OpenGLRenderManager&&) = delete;
public:
    void addRenderObject(DrawableObject&& _obj);
    void render();
    void setDefaultShaderProgram();
    void createCustomShaderProgam(string _vertexShader, string _fragmentShader,GLuint& _shaderID);
    void setCustomShaderProgram(GLuint& _shaderProgam);
    void setNullShaderProgram();
private:
    void createDefaultShaderProgram();


};


#endif //OPEN_SKETCH_OPENGLRENDERER_H
