//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_SHAPE_H
#define OPEN_SKETCH_SHAPE_H

#include "../OpenGL/DrawableObject.h"

class OpenGLRenderManager;

class Shape {
protected:
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_isSelected = false;

    DrawableObject m_drawable;
    OpenGLRenderManager* m_oglRenderer;
public:
    Shape(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height);
    virtual ~Shape();

    //Geen copies
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Shape(Shape&&) = default;
    Shape& operator=(Shape&&) = default;

    void setPosition(int _pixelsX, int _pixelsY);
    void setSize(int _width, int _height);

    inline void setSelected(bool _isSelected);
    inline bool getSelected() const;
    inline glm::mat4 getMatrix() const;
    inline glm::vec2 getPosition() const;
    inline glm::vec2 getSize() const;
    const DrawableObject& getDrawObject() const;
};

inline void Shape::setSelected(bool _isSelected)
{
    m_isSelected = _isSelected;
}

inline bool Shape::getSelected() const
{
    return m_isSelected;
}

inline glm::mat4 Shape::getMatrix() const
{
    return m_transformMatrix;
}

inline glm::vec2 Shape::getPosition() const
{
    return m_position;
}

inline glm::vec2 Shape::getSize() const
{
    return m_size;
}


#endif //OPEN_SKETCH_SHAPE_H
