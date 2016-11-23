//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_SUPPORTCLASSES_H
#define OPEN_SKETCH_SUPPORTCLASSES_H

#include "includeLibs.h"

//Args struct
struct Args
{
    int m_argc = 0;
    char** m_argv = nullptr;
};

//Characters struct
struct Character {
    GLuint m_texID;
    glm::ivec2 m_size;
    glm::ivec2 m_bearing;
    GLuint m_advance;
};



#endif //OPEN_SKETCH_SUPPORTCLASSES_H
