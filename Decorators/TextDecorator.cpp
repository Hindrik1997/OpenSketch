//
// Created by Hindrik Stegenga on 23-11-16.
//

#include "TextDecorator.h"
#include "../Application.h"

TextDecorator::TextDecorator(Shape* _shape) : ShapeDecorator(_shape){

}

void TextDecorator::draw() const {
    ShapeDecorator::draw();
    Application::getInstance().renderText("TEST", m_shape->getPosition().x, static_cast<GLfloat>(PAINT_WINDOW_SIZE_Y) - m_shape->getPosition().y, 0.2f, glm::vec3(0.0,0.0,0.0));
}
