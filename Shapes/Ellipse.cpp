//
// Created by Hindrik Stegenga on 29-9-16.
//

#include "Ellipse.h"

#include <algorithm>
#include "Rectangle.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "../Application.h"

Ellipse::Ellipse(OpenGLRenderManager* _oglRenderer,int _posx, int _posy, int _width, int _height) : Shape(_oglRenderer, _posx, _posy, _width, _height)
{
}

Ellipse::~Ellipse()
{
}