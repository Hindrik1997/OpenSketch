//
// Created by Hindrik Stegenga on 16-11-16.
//

#include "SetSelectedVisitor.h"
#include "../Shapes/Shape.h"

SetSelectedVisitor::SetSelectedVisitor(bool _val) : m_value(_val) {
}

void SetSelectedVisitor::start_visit(Shape &_shape) {
    call_automatic(&Shape::setSelected,_shape, m_value);
}

void SetSelectedVisitor::start_visit(Group &_group) {
    //Indirectly implemented by the recursion in the accept()
}

void SetSelectedVisitor::stop_visit(Shape &_shape) {
    //not necessary
}

void SetSelectedVisitor::stop_visit(Group &_group) {
    //not necessary
}