//
// Created by Hindrik Stegenga on 20-10-16.
//

#include "EllipseDrawer.h"

EllipseDrawer::EllipseDrawer() : Drawer(std::string("vertex_ellipse.glsl"), std::string("fragment_ellipse.glsl"))
{

}

EllipseDrawer &EllipseDrawer::getInstance() {
    static EllipseDrawer instance;
    return instance;
}

std::string EllipseDrawer::toString() const {
    return std::string(" ellipse");
}
