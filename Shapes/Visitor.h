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

template<typename T>
class Visitor
{
public:
    virtual void visit(T& _v) = 0;
    virtual ~Visitor() = 0;
};
template<typename T>
Visitor<T>::~Visitor(){}


#endif //OPEN_SKETCH_VISITOR_H
