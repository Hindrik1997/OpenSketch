//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_VISITABLE_H
#define OPEN_SKETCH_VISITABLE_H

/*
 * VISITABLE BASE CLASS,
 *
 * Definieert de accept() functie voor het visitor pattern.
 *
 * */

class Visitor;

class Visitable
{
public:
    virtual void accept(Visitor& _v) = 0;
    virtual ~Visitable() = 0;
};
inline Visitable::~Visitable(){}


#endif //OPEN_SKETCH_VISITABLE_H
