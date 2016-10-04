//
// Created by Hindrik Stegenga on 27-9-16.
//

#include <algorithm>
#include "Rectangle.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "../Application.h"

Rectangle::Rectangle(OpenGLRenderManager* _oglRenderer,int _posx, int _posy, int _width, int _height) : Shape(_oglRenderer, _posx, _posy, _width, _height)
{
}

Rectangle::~Rectangle() {
}