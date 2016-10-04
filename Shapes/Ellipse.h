//
// Created by Hindrik Stegenga on 29-9-16.
//

#ifndef OPEN_SKETCH_ELLIPSE_H
#define OPEN_SKETCH_ELLIPSE_H


#include "Shape.h"

class OpenGLRenderManager;

class Ellipse : public Shape
{
public:
    Ellipse(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height);
    ~Ellipse();

    //Geen copies
    Ellipse(const Ellipse&) = delete;
    Ellipse& operator=(const Ellipse&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Ellipse(Ellipse&&) = default;
    Ellipse& operator=(Ellipse&&) = default;
};

#endif //OPEN_SKETCH_ELLIPSE_H