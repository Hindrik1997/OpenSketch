//
// Created by Hindrik Stegenga on 16-11-16.
//

#include "DrawVisitor.h"
#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"
#include "../Decorators/ShapeDecorator.h"

void DrawVisitor::start_visit(Shape& _shape) {
    //selects the adequate possibly overloaded draw()
    call_automatic(&Shape::draw, _shape);
}

void DrawVisitor::start_visit(Group &_group) {
    call_automatic(&Shape::draw, dynamic_cast<Shape&>(_group));
}

void DrawVisitor::stop_visit(Shape &_shape) {
    //not necessary
}

void DrawVisitor::stop_visit(Group &_group) {
    //not necessary
}