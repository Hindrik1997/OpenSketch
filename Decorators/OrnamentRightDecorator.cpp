//
// Created by Hindrik Stegenga on 23-11-16.
//

#include "OrnamentRightDecorator.h"
#include "../Application.h"

void OrnamentRightDecorator::draw() const {
    ShapeDecorator::draw();
    Application::getInstance().renderText(m_text, m_shape->getPosition().x + (m_shape->getSize().x / 1.5f), PAINT_WINDOW_SIZE_Y - m_shape->getPosition().y, 0.2, glm::vec3(0.0f,0.0f,0.0f));
}

OrnamentRightDecorator::OrnamentRightDecorator(string _text, Shape *_shape) : ShapeDecorator(_shape), m_text(_text){

}
