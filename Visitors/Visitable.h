//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_VISITABLE_H
#define OPEN_SKETCH_VISITABLE_H

/*
 * VISITABLE BASE CLASS,
 *
 * Definieert de accept() functie voor het visitor pattern.
 * Base class, dus virtual destructor!
 * */

class Visitor;

class Visitable
{
public:
    virtual void accept(Visitor& _v) = 0;
    virtual ~Visitable() = 0;
};

//Altijd een implementatie, ookal is de destructor puur virtueel
inline Visitable::~Visitable(){}


#endif //OPEN_SKETCH_VISITABLE_H
