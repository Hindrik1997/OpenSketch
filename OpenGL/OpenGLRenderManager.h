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
#include "../Rectangle.h"
#include "DrawableObject.h"
#include "../includeLibs.h"
#include "../Application.h"

using namespace std;

class OpenGLRenderManager {
private:
    Application& m_application;
    GLuint m_defaultShaderProgram;
    vector<DrawableObject> m_objects;
    vector<Rectangle> m_rectangles;
public:
    OpenGLRenderManager(Application& _app);

    OpenGLRenderManager(const OpenGLRenderManager&) = delete;
    OpenGLRenderManager(const OpenGLRenderManager&&) = delete;
    void operator=(const OpenGLRenderManager&) = delete;
    void operator=(const OpenGLRenderManager&&) = delete;
public:
    //Returns nullptr if no rectangle is selected!
    Rectangle* getSelectedRectangle();

    //Prioritizes the rectangle specified
    Rectangle* getSelectedRectanglePriority(Rectangle* _rect);

    glm::vec2 getMouseOffsetInRectangle(Rectangle& _rect, int _mousex, int _mousey);


    void addRenderObject(vector<GLfloat> _verts, vector<GLint> _indices);
    void addRectangle(int _posx, int _posy, int _width, int _height);
    void render();
    void setDefaultShaderProgram();
    void createCustomShaderProgam(string _vertexShader, string _fragmentShader,GLuint& _shaderID);
    void setCustomShaderProgram(GLuint& _shaderProgam);
    void setNullShaderProgram();
    inline const vector<Rectangle>& getRectangles() const;
    inline const vector<DrawableObject>& getDrawables() const;
private:
    void createDefaultShaderProgram();

};

inline const vector<Rectangle>& OpenGLRenderManager::getRectangles() const
{
    return m_rectangles;
}

inline const vector<DrawableObject>& OpenGLRenderManager::getDrawables() const
{
    return m_objects;
}


#endif //OPEN_SKETCH_OPENGLRENDERER_H
