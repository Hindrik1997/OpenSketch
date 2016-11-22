//
// Created by Hindrik Stegenga on 22-11-16.
//

#ifndef OPEN_SKETCH_SHAPETESTDECORATOR_H
#define OPEN_SKETCH_SHAPETESTDECORATOR_H

#include "ShapeDecorator.h"

class ShapeTestDecorator : public ShapeDecorator {
public:
    ShapeTestDecorator(Shape* _shape);

    virtual void draw() const override;
};

#endif //OPEN_SKETCH_SHAPETESTDECORATOR_H