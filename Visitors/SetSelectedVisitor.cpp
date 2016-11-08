//
// Created by Hindrik Stegenga on 5-11-16.
//

#include "SetSelectedVisitor.h"

SetSelectedVisitor::SetSelectedVisitor(bool _value) : m_value(_value){
}

void SetSelectedVisitor::visit(Shape &_v) {
    _v.setSelected(m_value);
}

void SetSelectedVisitor::visit(Group &_v) {
    for(auto&& s : _v.getShapes())
    {
        s->setSelected(m_value);
    }
}
