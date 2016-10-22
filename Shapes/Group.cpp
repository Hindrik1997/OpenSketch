//
// Created by Hindrik Stegenga on 21-10-16.
//

#include "Group.h"
#include <algorithm>

Group::Group(OpenGLRenderManager* _oglRenderer) : Shape(_oglRenderer, 0,0,0,0, nullptr)
{
}

Shape& Group::addShape(OpenGLRenderManager *_oglRenderer, int _posx, int _posy, int _width, int _height, Drawer *_drawer) {
    m_shapes.emplace_back(_oglRenderer,_posx, _posy, _width, _height, _drawer);
    return m_shapes.back();
}

void Group::removeShape(Shape* _shape) {
    m_shapes.erase(std::remove_if(m_shapes.begin(), m_shapes.end(), [_shape](const Shape& i) -> bool { return &i == _shape; }), m_shapes.end());
}

size_t Group::getShapeCount() const {
    return m_shapes.size();
}

void Group::draw() const {
    for(auto&& s : m_shapes)
        s.draw();
}

Shape* Group::getSelectedShape(int _xpos, int _ypos) const {
    Shape* found = nullptr;
    for(auto&& s : m_shapes) {
        Shape* t = s.getSelectedShape(_xpos, _ypos);
        if(t != nullptr)
            found = t;
    }
    return found;
}

vector<int> Group::getIndex(Shape *_shape) {

    vector<int> indices;

    for(int i = 0; i < static_cast<int>(m_shapes.size()); ++i)
    {
        Shape& s = m_shapes[i];
        if(typeid(s) == typeid(Group))
        {
            //if group
            vector<int> result = dynamic_cast<Group&>(s).getIndex(_shape);
            if(result.size() == 0)
                continue;

            indices.push_back(i);
            indices.insert(std::end(indices), std::begin(result), std::end(result));
        } else
        {
            //just a shape
            if(&s == _shape)
            {
                indices.push_back(i);
            }
        }
    }
    return indices;
}

Shape &Group::operator[](const int _index) {
    return m_shapes[_index];
}

vector<Shape> &Group::getShapeVector() {
    return m_shapes;
}

Shape *Group::getSelectedShapePriority(int _xpos, int _ypos, Shape *_prioShape, bool& _isPrio) {
    Shape* found = nullptr;
    for(auto&& s : m_shapes) {
        Shape* t = s.getSelectedShapePriority(_xpos, _ypos, _prioShape, _isPrio);
        if(_isPrio)
            return _prioShape;
        if(t != nullptr)
            found = t;
    }
    return found;
}
