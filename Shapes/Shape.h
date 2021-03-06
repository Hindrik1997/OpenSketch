//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_SHAPE_H
#define OPEN_SKETCH_SHAPE_H

#include <typeinfo>
#include <string>
#include <vector>
#include "../OpenGL/DrawableObject.h"
#include "../Visitors/Visitable.h"

class Drawer;
class ShapeRenderManager;

using std::string;
using std::vector;
using std::to_string;

class Shape : public Visitable {
protected:
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    glm::vec2 m_position= glm::vec2(0.0f);
    glm::vec2 m_size = glm::vec2(0.0f);
    bool m_isSelected = false;

    Drawer* m_drawer = nullptr;
public:
    Shape(int _posx, int _posy, int _width, int _height, Drawer* _drawer);
    virtual ~Shape();

    //Geen copies
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Shape(Shape&&) = default;
    Shape& operator=(Shape&&) = default;

    virtual void draw() const;

    virtual void resize(int _pixelsX, int _pixelsY);
    virtual void move(int _pixelsX, int _pixelsY);

    virtual inline void setSelected(bool _isSelected);
    virtual inline bool getSelected() const;
    virtual inline glm::mat4 getMatrix() const;

    virtual void accept(Visitor &_v) override;

    virtual inline glm::vec2 getPosition() const;
    virtual inline glm::vec2 getSize() const;

    virtual vector<string> getDrawInformation() const;

    virtual inline Drawer* getDrawer() const;
};

//inlined member functies

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

inline Drawer* Shape::getDrawer() const {
    return m_drawer;
}

#endif //OPEN_SKETCH_SHAPE_H
