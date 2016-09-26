//
// Created by Hindrik Stegenga on 26-9-16.
//

#ifndef OPEN_SKETCH_DRAWABLEOBJECT_H
#define OPEN_SKETCH_DRAWABLEOBJECT_H

#include "../includeLibs.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

class DrawableObject {
private:
    GLuint m_VBO = 0, m_EBO = 0, m_VAO = 0;
    vector<GLfloat> m_vertices;
    vector<GLint> m_indices;
public:
    DrawableObject(vector<GLfloat> _vertices, vector<GLint> _indices);
    ~DrawableObject();
private:


public:
    void resetObject();
    void resetObject(vector<GLfloat> _vertices, vector<GLint> _indices);
    void draw();
};

#endif //OPEN_SKETCH_DRAWABLEOBJECT_H
