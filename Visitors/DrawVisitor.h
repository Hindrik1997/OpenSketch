//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_DRAWVISITOR_H
#define OPEN_SKETCH_DRAWVISITOR_H


#include "../Shapes/Visitor.h"
#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"

class DrawVisitor : public virtual Visitor<Shape>, public virtual Visitor<Group>{
public:
    virtual void visit(Shape&_v) override;

    virtual void visit(Group& _v) override;

};


#endif //OPEN_SKETCH_DRAWVISITOR_H
