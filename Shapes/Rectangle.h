//
// Created by Hindrik Stegenga on 27-9-16.
//

#ifndef OPEN_SKETCH_RECTANGLE_H
#define OPEN_SKETCH_RECTANGLE_H

#include "Shape.h"

class OpenGLRenderManager;

class Rectangle : public Shape
{
public:
    Rectangle(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height);
    ~Rectangle();

    //Geen copies
    Rectangle(const Rectangle&) = delete;
    Rectangle& operator=(const Rectangle&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    Rectangle(Rectangle&&) = default;
    Rectangle& operator=(Rectangle&&) = default;
};

#endif //OPEN_SKETCH_RECTANGLE_H