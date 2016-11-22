//
// Created by Hindrik Stegenga on 22-11-16.
//

#include "Visitor.h"
#include "../Decorators/ShapeDecorator.h"


void Visitor::start_visit(ShapeDecorator &_decorator) {
    //pushes pointer to the overriding decorator on stack
    Shape* s = &_decorator;
    m_deque.push_back(s);
}

void Visitor::stop_visit(ShapeDecorator &_decorator) {
    //pops the pointer
    m_deque.pop_back();
}