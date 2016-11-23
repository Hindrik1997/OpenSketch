//
// Created by Hindrik Stegenga on 23-11-16.
//

#ifndef OPEN_SKETCH_ORNAMENTBOTTOMDECORATOR_H
#define OPEN_SKETCH_ORNAMENTBOTTOMDECORATOR_H

#include "ShapeDecorator.h"
#include <string>

class OrnamentBottomDecorator : public ShapeDecorator {
private:
    string m_text;
public:
    OrnamentBottomDecorator(string _text, Shape* _shape);

    void draw() const override;
};


#endif //OPEN_SKETCH_ORNAMENTBOTTOMDECORATOR_H
