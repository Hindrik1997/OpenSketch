//
// Created by Hindrik Stegenga on 20-11-16.
//

#ifndef OPEN_SKETCH_SHAPEINTERFACE_H
#define OPEN_SKETCH_SHAPEINTERFACE_H

#include "../includeLibs.h"

class Drawer;

class IShape
{
    virtual void draw() const = 0;
    virtual void resize(int _pixelsX, int _pixelsY) = 0;
    virtual void move(int _pixelsX, int _pixelsY) = 0;

    virtual bool getSelected() const = 0;
    virtual glm::vec2 getPosition() const = 0;
    virtual glm::vec2 getSize() const = 0;
    virtual Drawer* getDrawer() const = 0;
};


#endif //OPEN_SKETCH_SHAPEINTERFACE_H
