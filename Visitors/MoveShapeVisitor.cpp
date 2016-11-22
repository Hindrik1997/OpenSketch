//
// Created by hindrik on 14-11-16.
//

#include "MoveShapeVisitor.h"
#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"

MoveShapeVisitor::MoveShapeVisitor(int _x, int _y) : m_x(_x), m_y(_y){
}

void MoveShapeVisitor::start_visit(Shape &_shape) {
    _shape.move(m_x, m_y);
}

void MoveShapeVisitor::start_visit(Group &_group) {
    //can be left empty, the group's accept already manages this by forwarding it all until it ends up on a shape
}

void MoveShapeVisitor::stop_visit(Shape &_shape) {
    //Not necessary
}

void MoveShapeVisitor::stop_visit(Group &_group) {
    //Not necessary
}