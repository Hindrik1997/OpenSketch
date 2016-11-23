//
// Created by Hindrik Stegenga on 23-11-16.
//

#ifndef OPEN_SKETCH_ORNAMENTTOPDECORATOR_H
#define OPEN_SKETCH_ORNAMENTTOPDECORATOR_H


#include "ShapeDecorator.h"
#include <string>

class OrnamentTopDecorator : public ShapeDecorator {
private:
    string m_text;
public:
    OrnamentTopDecorator(string _text, Shape* _shape);

    void draw() const override;
};


#endif //OPEN_SKETCH_ORNAMENTTOPDECORATOR_H
