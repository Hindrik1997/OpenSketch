//
// Created by hindrik on 14-11-16.
//

#ifndef OPEN_SKETCH_MOVESHAPEVISITOR_H
#define OPEN_SKETCH_MOVESHAPEVISITOR_H


#include "Visitor.h"
#include <vector>
#include <string>


/*
 * Visitor implementation that moves all shapes by m_x and m_y pixels.  
 * */
class MoveShapeVisitor : public Visitor{
private:
    int m_x = 0;
    int m_y = 0;
public:
    MoveShapeVisitor(int _x, int _y);

    virtual void start_visit(Shape &_shape) override;

    virtual void start_visit(Group &_group) override;

    virtual void stop_visit(Shape &_shape) override;

    virtual void stop_visit(Group &_group) override;
};




#endif //OPEN_SKETCH_MOVESHAPEVISITOR_H
