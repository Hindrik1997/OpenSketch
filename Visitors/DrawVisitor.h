//
// Created by Hindrik Stegenga on 16-11-16.
//

#ifndef OPEN_SKETCH_DRAWVISITOR_H
#define OPEN_SKETCH_DRAWVISITOR_H


#include "Visitor.h"
#include <deque>

using std::deque;

/*
 *
 * Visitor die de drawing implementeert.
 *
 *
 * */

class DrawVisitor : public Visitor {
public:
    virtual void start_visit(Shape &_shape) override;

    virtual void start_visit(Group &_group) override;

    virtual void stop_visit(Shape &_shape) override;

    virtual void stop_visit(Group &_group) override;

};


#endif //OPEN_SKETCH_DRAWVISITOR_H
