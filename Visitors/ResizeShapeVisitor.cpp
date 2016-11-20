//
// Created by hindrik on 16-11-16.
//

#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"
#include "ResizeShapeVisitor.h"
#include "MoveShapeVisitor.h"

ResizeShapeVisitor::ResizeShapeVisitor(int _pixelsX, int _pixelsY) : m_pixelsX(_pixelsX), m_pixelsY(_pixelsY)
{
}

void ResizeShapeVisitor::start_visit(Shape &_shape) {
    if(m_stack.size() == 0)
    {
        _shape.resize(m_pixelsX, m_pixelsY);
        return;
    }

    ResizeData& rs = m_stack.top();
    int newHeight = static_cast<int>(_shape.getSize().y * rs.m_factorY);
    int newWidth = static_cast<int>(_shape.getSize().x * rs.m_factorX);

    int delta_x = static_cast<int>((_shape.getPosition().x - rs.m_old_width) * (rs.m_factorX - 1));
    int delta_y = static_cast<int>((_shape.getPosition().y - rs.m_old_height) * (rs.m_factorY - 1));

    MoveShapeVisitor v(delta_x, delta_y);
    _shape.accept(v);

    int changeX = newWidth - static_cast<int>(_shape.getSize().x);
    int changeY = newHeight - static_cast<int>(_shape.getSize().y);

    //int changeX = newWidth > 0 ? newWidth - static_cast<int>(_shape.getSize().x) : 0;
    //int changeY = newHeight > 0 ? newHeight - static_cast<int>(_shape.getSize().y) : 0;

    if(!(changeX == 0 && changeY == 0))
        _shape.resize(changeX, changeY);
}

void ResizeShapeVisitor::start_visit(Group& _group) {

    int old_pos_x = static_cast<int>(_group.getPosition().x);
    int old_pos_y = static_cast<int>(_group.getPosition().y);

    int old_width = static_cast<int>(_group.getSize().x);
    int old_height = static_cast<int>(_group.getSize().y);

    int px = m_stack.size() == 0 ? m_pixelsX : m_stack.top().m_pixelsX;
    int py = m_stack.size() == 0 ? m_pixelsY : m_stack.top().m_pixelsY;

    float factorX = static_cast<float>(old_width + px) / old_width;
    float factorY = static_cast<float>(old_height + py) / old_height;

    ResizeData rs = {
        px,
        py,
        old_pos_x,
        old_pos_y,
        old_width,
        old_height,
        factorX,
        factorY
    };

    m_stack.push(rs);
}

void ResizeShapeVisitor::stop_visit(Shape &_shape) {

}

void ResizeShapeVisitor::stop_visit(Group &_group) {

    ResizeData rs = m_stack.top();

    int xdiff = static_cast<int>(rs.m_old_pos_x - _group.getPosition().x);
    int ydiff = static_cast<int>(rs.m_old_pos_y - _group.getPosition().y);

    MoveShapeVisitor v(xdiff,ydiff);
    _group.accept(v);
    m_stack.pop();
}