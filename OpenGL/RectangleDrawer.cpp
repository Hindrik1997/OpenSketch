//
// Created by Hindrik Stegenga on 20-10-16.
//

#include "RectangleDrawer.h"

RectangleDrawer::RectangleDrawer() : Drawer(std::string("vertex_rectangle.glsl"), std::string("fragment_rectangle.glsl"))
{
}

RectangleDrawer &RectangleDrawer::getInstance() {
    static RectangleDrawer instance;
    return instance;
}

std::string RectangleDrawer::toString() const {
    return std::string("RectangleDrawer: draws rectangles");
}