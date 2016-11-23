//
// Created by Hindrik Stegenga on 23-11-16.
//

#ifndef OPEN_SKETCH_TEXTDECORATOR_H
#define OPEN_SKETCH_TEXTDECORATOR_H

#include "ShapeDecorator.h"
#include <string>

class TextDecorator : public ShapeDecorator {
public:
    TextDecorator(Shape* _shape);

    void draw() const override;
private:


};


#endif //OPEN_SKETCH_TEXTDECORATOR_H
