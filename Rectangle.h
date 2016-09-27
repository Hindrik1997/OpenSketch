//
// Created by Hindrik Stegenga on 27-9-16.
//

#ifndef OPEN_SKETCH_RECTANGLE_H
#define OPEN_SKETCH_RECTANGLE_H

#include "OpenGL/DrawableObject.h"

class OpenGLRenderManager;

class Rectangle {
private:
    glm::vec4 m_position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    int m_drawableObjectindex;
    OpenGLRenderManager& m_oglRenderer;
public:
    Rectangle(OpenGLRenderManager& _oglRenderer, int _posx, int _posy, int _width, int _height);
    void setPosition(int _pixelsX, int _pixelsY);
    void setSize(int _width, int _height);

    inline glm::mat4 getMatrix() const;
    inline glm::vec4 getPosition() const;
    const DrawableObject& getDrawObject() const;
};

inline glm::mat4 Rectangle::getMatrix() const
{
    return m_transformMatrix;
}

inline glm::vec4 Rectangle::getPosition() const
{
    return m_position;
}
#endif //OPEN_SKETCH_RECTANGLE_H
