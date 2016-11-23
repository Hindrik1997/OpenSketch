//
// Created by Hindrik Stegenga on 23-11-16.
//

#include "OrnamentBottomDecorator.h"
#include "../Application.h"
#include "../Shapes/Visitor.h"

void OrnamentBottomDecorator::draw() const {
    ShapeDecorator::draw();
    Application::getInstance().renderText(m_text, m_shape->getPosition().x - m_shape->getSize().x / 2, (PAINT_WINDOW_SIZE_Y - m_shape->getPosition().y) - m_shape->getSize().y / 1.2f, 0.2, glm::vec3(0.0f,0.0f,0.0f));
}

OrnamentBottomDecorator::OrnamentBottomDecorator(string _text, Shape *_shape) : ShapeDecorator(_shape), m_text(_text){

}

vector<string> OrnamentBottomDecorator::getDrawInformation() const {
    vector<string> data;
    string t = "ornament bottom \"";
    t.append(m_text);
    t.append("\"");
    data.push_back(t);
    vector<string> result = ShapeDecorator::getDrawInformation();
    data.insert(std::end(data), std::begin(result), std::end(result));
    return data;
}