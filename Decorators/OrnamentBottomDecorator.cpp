//
// Created by Hindrik Stegenga on 23-11-16.
//

#include "OrnamentBottomDecorator.h"
#include "../Application.h"

void OrnamentBottomDecorator::draw() const {
    ShapeDecorator::draw();
    Application::getInstance().renderText(m_text, m_shape->getPosition().x - m_shape->getSize().x / 2, (PAINT_WINDOW_SIZE_Y - m_shape->getPosition().y) - m_shape->getSize().y / 1.2f, 0.2, glm::vec3(0.0f,0.0f,0.0f));
}

OrnamentBottomDecorator::OrnamentBottomDecorator(string _text, Shape *_shape) : ShapeDecorator(_shape), m_text(_text){

}
