//
// Created by Hindrik Stegenga on 26-9-16.
//

#ifndef OPEN_SKETCH_OPENGLRENDERER_H
#define OPEN_SKETCH_OPENGLRENDERER_H

#include "../includeLibs.h"
#include "../Application.h"

class OpenGLRenderManager {
private:
    Application& m_Application;
    GLuint m_defaultShaderProgam;



public:
    OpenGLRenderManager(Application& _app);



public:
    void render();
private:
    void createDefaultShaders();




};


#endif //OPEN_SKETCH_OPENGLRENDERER_H
