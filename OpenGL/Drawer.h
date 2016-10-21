//
// Created by Hindrik Stegenga on 20-10-16.
//

#ifndef OPEN_SKETCH_DRAWER_H
#define OPEN_SKETCH_DRAWER_H

#include <string>
#include "OpenGLRenderManager.h"

class Drawer {
private:
    GLuint m_shader;
protected:
    Drawer(std::string _vshader, std::string _fshader);
public:
    virtual void draw(Shape& _shape) const;
    virtual ~Drawer() = 0;
};



#endif //OPEN_SKETCH_DRAWER_H
