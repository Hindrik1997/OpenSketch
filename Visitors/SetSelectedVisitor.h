//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_SETSELECTEDVISITOR_H
#define OPEN_SKETCH_SETSELECTEDVISITOR_H

#include "../Shapes/Visitor.h"
#include "../Shapes/Shape.h"
#include "../Shapes/Group.h"

class SetSelectedVisitor : public virtual Visitor<Shape>, public virtual Visitor<Group>{
private:
    bool m_value = false;
public:
    SetSelectedVisitor(bool _value);

    virtual void visit(Shape&_v) override;

    virtual void visit(Group& _v) override;

};


#endif //OPEN_SKETCH_SETSELECTEDVISITOR_H
