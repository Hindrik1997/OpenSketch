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
#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"
#include "DrawableObject.h"
#include "../includeLibs.h"
#include "../Application.h"

using namespace std;

class OpenGLRenderManager {
private:
    Application& m_application;
    vector<unique_ptr<Shape>> m_shapes;
public:
    OpenGLRenderManager(Application& _app);

    OpenGLRenderManager(const OpenGLRenderManager&) = delete;
    OpenGLRenderManager(const OpenGLRenderManager&&) = delete;
    void operator=(const OpenGLRenderManager&) = delete;
    void operator=(const OpenGLRenderManager&&) = delete;
public:
    //Returns nullptr if no rectangle/ellipse is selected!
    Shape* getSelectedShape();
    Shape* getSelectedShapePriority(Shape* _shape);

    size_t getIndex(Shape* _shape);

    static glm::vec2 getMouseOffsetInShape(Shape& _rect, int _mousex, int _mousey);

    Shape& addShape(int _posx, int _posy, int _width, int _height, Drawer* _drawer);

    void createGroup(std::vector<Shape*>& _set);

    void render();
    static void createCustomShaderProgam(string _vertexShader, string _fragmentShader, GLuint& _shaderID);
    static void setCustomShaderProgram(const GLuint& _shaderProgam);
    static void setNullShaderProgram();
    inline const vector<unique_ptr<Shape>>& getShapes() const;
};

inline const vector<unique_ptr<Shape>>& OpenGLRenderManager::getShapes() const
{
    return m_shapes;
};

#endif //OPEN_SKETCH_OPENGLRENDERER_H
