//
// Created by Hindrik Stegenga on 21-10-16.
//

#include "Group.h"
#include <algorithm>

Group::Group(OpenGLRenderManager* _oglRenderer, vector<unique_ptr<Shape>>& _vec) : Shape(_oglRenderer, 0,0,0,0, nullptr)
{
    for(auto&& s : _vec)
    {
        m_shapes.emplace_back(std::move(s));
    }
    _vec.clear();
}

void Group::draw() const {
    for(auto&& s : m_shapes)
        s->draw();
}

void Group::setPosition(int _pixelsX, int _pixelsY) {
    Shape::setPosition(_pixelsX, _pixelsY);
}

void Group::setSize(int _width, int _height) {
    Shape::setSize(_width, _height);
}

glm::mat4 Group::getMatrix() const {
    return Shape::getMatrix();
}

glm::vec2 Group::getPosition() const {
    return Shape::getPosition();
}

glm::vec2 Group::getSize() const {
    return Shape::getSize();
}

vector<unique_ptr<Shape>>& Group::getShapes() {
    return m_shapes;
}
