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

template<typename T>
class Visitor;

template<typename T>
class Visitable
{
public:
    virtual void accept(Visitor<T>& _v) = 0;
    virtual ~Visitable() = 0;
};
template<typename T>
Visitable<T>::~Visitable(){}


#endif //OPEN_SKETCH_VISITABLE_H
