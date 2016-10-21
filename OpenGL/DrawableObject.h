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

//SINGLETON PATTERN

class DrawableObject {
private:
    GLuint m_VBO = 0, m_EBO = 0, m_VAO = 0;
    vector<GLfloat> m_vertices;
    vector<GLint> m_indices;
private:
    DrawableObject(vector<GLfloat> _vertices, vector<GLint> _indices);
public:
    ~DrawableObject();

    //Geen copies
    DrawableObject(const DrawableObject&) = delete;
    DrawableObject& operator=(const DrawableObject&) = delete;

    //Wel moven, immers het deleten van de copy ctor delete de move ctor impliciet, dus die zetten
    //we expliciet op default!
    DrawableObject(DrawableObject&&) = default;
    DrawableObject& operator=(DrawableObject&&) = default;
public:
    static DrawableObject& getInstance();
    void resetObject();
    void resetObject(vector<GLfloat> _vertices, vector<GLint> _indices);
    void draw() const;
};

#endif //OPEN_SKETCH_DRAWABLEOBJECT_H
