//
// Created by Hindrik Stegenga on 29-9-16.
//

#ifndef OPEN_SKETCH_ELLIPSE_H
#define OPEN_SKETCH_ELLIPSE_H


#include "../OpenGL/DrawableObject.h"

class OpenGLRenderManager;

class Ellipse {
private:
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_isSelected = false;

    DrawableObject m_drawable;
    OpenGLRenderManager* m_oglRenderer;
public:
    Ellipse(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height);
    ~Ellipse();

    //Geen copies
    Ellipse(const Ellipse&) = delete;
    Ellipse& operator=(const Ellipse&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Ellipse(Ellipse&&) = default;
    Ellipse& operator=(Ellipse&&) = default;

    void setPosition(int _pixelsX, int _pixelsY);
    void setSize(int _width, int _height);

    inline void setSelected(bool _isSelected);
    inline bool getSelected() const;
    inline glm::mat4 getMatrix() const;
    inline glm::vec2 getPosition() const;
    inline glm::vec2 getSize() const;
    const DrawableObject& getDrawObject() const;
};

inline void Ellipse::setSelected(bool _isSelected)
{
    m_isSelected = _isSelected;
}

inline bool Ellipse::getSelected() const
{
    return m_isSelected;
}

inline glm::mat4 Ellipse::getMatrix() const
{
    return m_transformMatrix;
}

inline glm::vec2 Ellipse::getPosition() const
{
    return m_position;
}

inline glm::vec2 Ellipse::getSize() const
{
    return m_size;
}




#endif //OPEN_SKETCH_ELLIPSE_H
