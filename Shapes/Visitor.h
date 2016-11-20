//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_VISITOR_H
#define OPEN_SKETCH_VISITOR_H

/*
 * VISITOR BASE CLASS
 *  Base class voor visitor implementaties.
 *  start_visit() en stop_visit() moeten in elke implementatie ge-overide worden. Ookal laat je deze leeg, override ze!
 *  (Je kan niet anders in c++, omdat de klasse anders abstract is, maar ik zeg het er maar ff bij)
 */

class Shape;
class Group;


class Visitor
{
public:
    virtual void start_visit(Shape &_shape) = 0;
    virtual void start_visit(Group &_group) = 0;

    virtual void stop_visit(Shape& _shape) = 0;
    virtual void stop_visit(Group& _group) = 0;

    virtual ~Visitor() = 0;
};

//Ook hier weer een implementatie, omdat deze verplicht is, zelfs als deze puur virtueel is.
inline Visitor::~Visitor(){}


#endif //OPEN_SKETCH_VISITOR_H