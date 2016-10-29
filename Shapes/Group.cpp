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

    int old_width = static_cast<int>(getSize().x);
    int old_height = static_cast<int>(getSize().y);







}

glm::vec2 Group::getPosition() const {
    glm::vec2 result;
    int xmin,xmax,ymin,ymax;
    calculateMetrics(xmin,xmax,ymin,ymax);

    result.y = (ymin + ymax) / 2;
    result.x = (xmin + xmax) / 2;
    return result;
}

glm::vec2 Group::getSize() const {
    glm::vec2 result;
    int xmin,xmax,ymin,ymax;
    calculateMetrics(xmin,xmax,ymin,ymax);
    result.x = xmax - xmin;
    result.y = ymax - ymin;
    return result;
}

vector<unique_ptr<Shape>>& Group::getShapes() {
    return m_shapes;
}

void Group::calculateMetrics(int& _minx, int& _maxx, int& _miny, int& _maxy) const {

    int min_x = std::numeric_limits<int>::max(),
            max_x = std::numeric_limits<int>::min(),
            min_y = std::numeric_limits<int>::max(),
            max_y = std::numeric_limits<int>::min();

    for(auto&& s : m_shapes)
    {
        int txmin = static_cast<int>(s->getPosition().x - (s->getSize().x / 2)),
                tymin = static_cast<int>(s->getPosition().y - (s->getSize().y / 2)),
                txmax = static_cast<int>(s->getPosition().x + (s->getSize().x / 2)),
                tymax = static_cast<int>(s->getPosition().y + (s->getSize().y / 2));

        if(txmin < min_x) min_x = txmin;
        if(txmax > max_x) max_x = txmax;
        if(tymin < min_y) min_y = tymin;
        if(tymax > max_y) max_y = tymax;
    }
    _minx = min_x;
    _maxx = max_x;
    _miny = min_y;
    _maxy = max_y;
}

void Group::setSelected(bool _isSelected) {
    Shape::setSelected(_isSelected);
    for(auto&& s : m_shapes)
    {
        s->setSelected(_isSelected);
    }
}

bool Group::getSelected() const {
    return Shape::getSelected();
}