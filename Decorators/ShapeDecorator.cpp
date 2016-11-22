//
// Created by Hindrik Stegenga on 20-11-16.
//

#include "ShapeDecorator.h"
#include "../Shapes/Visitor.h"

ShapeDecorator::ShapeDecorator(Shape* _shape) : m_shape(_shape), Shape(0,0,0,0,nullptr)
{
}

void ShapeDecorator::draw() const {
    m_shape->draw();
}

void ShapeDecorator::resize(int _pixelsX, int _pixelsY) {
    m_shape->resize(_pixelsX, _pixelsY);
}

void ShapeDecorator::move(int _pixelsX, int _pixelsY) {
    m_shape->move(_pixelsX, _pixelsY);
}

void ShapeDecorator::setSelected(bool _isSelected) {
    m_shape->setSelected(_isSelected);
}

bool ShapeDecorator::getSelected() const {
    return m_shape->getSelected();
}

glm::mat4 ShapeDecorator::getMatrix() const {
    return m_shape->getMatrix();
}

void ShapeDecorator::accept(Visitor &_v) {
    _v.start_visit(*this);
    m_shape->accept(_v);
    _v.stop_visit(*this);
}

glm::vec2 ShapeDecorator::getPosition() const {
    return m_shape->getPosition();
}

glm::vec2 ShapeDecorator::getSize() const {
    return m_shape->getSize();
}

Drawer *ShapeDecorator::getDrawer() const {
    return m_shape->getDrawer();
}
