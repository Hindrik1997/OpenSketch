//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_VISITOR_H
#define OPEN_SKETCH_VISITOR_H

/*
 * VISITOR BASE CLASS
 *  Base class voor visitor implementaties.
 *
 */

class Shape;
class Group;


class Visitor
{
public:
    virtual void visit(Shape& _shape) = 0;
    virtual void visit(Group& _group) = 0;
    virtual ~Visitor() = 0;
};

inline Visitor::~Visitor(){}


#endif //OPEN_SKETCH_VISITOR_H