//
// Created by Hindrik Stegenga on 5-11-16.
//

#include "DrawVisitor.h"

void DrawVisitor::visit(Shape &_v) {
    _v.draw();
}

void DrawVisitor::visit(Group &_v) {
    for(auto&& s : _v.getShapes())
        s->draw();
}