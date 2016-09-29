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
#include "../Ellipse.h"
#include "../Rectangle.h"
#include "DrawableObject.h"
#include "../includeLibs.h"
#include "../Application.h"

using namespace std;

class OpenGLRenderManager {
private:
    Application& m_application;
    GLuint m_defaultShaderProgramRectangle;
    GLuint m_defaultShaderProgramEllipse;
    vector<Rectangle> m_rectangles;
    vector<Ellipse> m_ellipses;
public:
    OpenGLRenderManager(Application& _app);

    OpenGLRenderManager(const OpenGLRenderManager&) = delete;
    OpenGLRenderManager(const OpenGLRenderManager&&) = delete;
    void operator=(const OpenGLRenderManager&) = delete;
    void operator=(const OpenGLRenderManager&&) = delete;
public:
    //Returns nullptr if no rectangle/ellipse is selected!
    Rectangle* getSelectedRectangle();
    Ellipse* getSelectedEllipse();

    //Prioritizes the rectangle/ellipse specified
    Rectangle* getSelectedRectanglePriority(Rectangle* _rect);
    Ellipse* getSelectedEllipsePriority(Ellipse* _ellipse);

    glm::vec2 getMouseOffsetInRectangle(Rectangle& _rect, int _mousex, int _mousey);
    glm::vec2 getMouseOffsetInEllipse(Ellipse& _ellipse, int _mousex, int _mousey);

    void addRectangle(int _posx, int _posy, int _width, int _height);
    void addEllipse(int _posx, int _posy, int _width, int _height);

    void render();
    void createCustomShaderProgam(string _vertexShader, string _fragmentShader,GLuint& _shaderID);
    void setCustomShaderProgram(GLuint& _shaderProgam);
    void setNullShaderProgram();
    inline const vector<Rectangle>& getRectangles() const;
    inline const vector<Ellipse>& getEllipses() const;
private:
    void createDefaultShaderProgram();

};

inline const vector<Rectangle>& OpenGLRenderManager::getRectangles() const
{
    return m_rectangles;
}

inline const vector<Ellipse>& OpenGLRenderManager::getEllipses() const
{
    return m_ellipses;
}


#endif //OPEN_SKETCH_OPENGLRENDERER_H
