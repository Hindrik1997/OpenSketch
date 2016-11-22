//
// Created by Hindrik Stegenga on 16-11-16.
//

#include "DrawVisitor.h"
#include "../Shapes/Shape.h"
#include "../Decorators/ShapeDecorator.h"

void DrawVisitor::start_visit(Shape& _shape) {
    //selects the adequate possibly overloaded draw()

    call_automatic(_shape, &Shape::draw);

}

void DrawVisitor::start_visit(Group &_group) {
    //Indirectly implemented by accept()
}

void DrawVisitor::stop_visit(Shape &_shape) {
    //not necessary
}

void DrawVisitor::stop_visit(Group &_group) {
    //not necessary
}