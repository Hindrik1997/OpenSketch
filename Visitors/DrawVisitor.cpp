//
// Created by Hindrik Stegenga on 16-11-16.
//

#include "DrawVisitor.h"
#include "../Shapes/Shape.h"

void DrawVisitor::start_visit(Shape &_shape) {
    _shape.draw();
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
