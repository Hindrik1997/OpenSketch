//
// Created by Hindrik Stegenga on 16-11-16.
//

#ifndef OPEN_SKETCH_SETSELECTEDVISITOR_H
#define OPEN_SKETCH_SETSELECTEDVISITOR_H


#include "../Shapes/Visitor.h"

/*
 * Implementeert selectie setten van shapes en groups.
 * */

class SetSelectedVisitor : public Visitor {
private:
    bool m_value = false;
public:
    SetSelectedVisitor(bool _val);

    virtual void start_visit(Shape &_shape) override;

    virtual void start_visit(Group &_group) override;

    virtual void stop_visit(Shape &_shape) override;

    virtual void stop_visit(Group &_group) override;
};


#endif //OPEN_SKETCH_SETSELECTEDVISITOR_H
