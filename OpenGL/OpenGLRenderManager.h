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
    Group m_rootGroup;
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

    vector<int> getIndex(Shape* _shape);

    static glm::vec2 getMouseOffsetInShape(Shape& _rect, int _mousex, int _mousey);

    Shape& addShape(int _posx, int _posy, int _width, int _height, Drawer* _drawer);

    void render();
    static void createCustomShaderProgam(string _vertexShader, string _fragmentShader, GLuint& _shaderID);
    static void setCustomShaderProgram(const GLuint& _shaderProgam);
    static void setNullShaderProgram();
    inline const Group& getRootGroup() const;
};

inline const Group& OpenGLRenderManager::getRootGroup() const
{
    return m_rootGroup;
};

#endif //OPEN_SKETCH_OPENGLRENDERER_H
