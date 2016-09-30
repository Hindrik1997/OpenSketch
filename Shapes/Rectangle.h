//
// Created by Hindrik Stegenga on 27-9-16.
//

#ifndef OPEN_SKETCH_RECTANGLE_H
#define OPEN_SKETCH_RECTANGLE_H

#include "../OpenGL/DrawableObject.h"

class OpenGLRenderManager;

class Rectangle {
private:
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_isSelected = false;

    DrawableObject m_drawable;
    OpenGLRenderManager* m_oglRenderer;
public:
    Rectangle(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height);
    ~Rectangle();

    //Geen copies
    Rectangle(const Rectangle&) = delete;
    Rectangle& operator=(const Rectangle&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Rectangle(Rectangle&&) = default;
    Rectangle& operator=(Rectangle&&) = default;

    void setPosition(int _pixelsX, int _pixelsY);
    void setSize(int _width, int _height);

    inline void setSelected(bool _isSelected);
    inline bool getSelected() const;
    inline glm::mat4 getMatrix() const;
    inline glm::vec2 getPosition() const;
    inline glm::vec2 getSize() const;
    const DrawableObject& getDrawObject() const;
};

inline void Rectangle::setSelected(bool _isSelected)
{
    m_isSelected = _isSelected;
}

inline bool Rectangle::getSelected() const
{
    return m_isSelected;
}

inline glm::mat4 Rectangle::getMatrix() const
{
    return m_transformMatrix;
}

inline glm::vec2 Rectangle::getPosition() const
{
    return m_position;
}

inline glm::vec2 Rectangle::getSize() const
{
    return m_size;
}



#endif //OPEN_SKETCH_RECTANGLE_H
