//
// Created by Hindrik Stegenga on 21-10-16.
//

#ifndef OPEN_SKETCH_GROUP_H
#define OPEN_SKETCH_GROUP_H

#include <typeinfo>
#include <vector>
#include "Shape.h"

using std::vector;

class Group : public Shape {
private:
    vector<Shape> m_shapes;
public:
    Group(OpenGLRenderManager* _oglRenderer);

    Shape& operator[] (const int _index);

    Shape& addShape(OpenGLRenderManager* _oglRenderer, int _posx, int _posy, int _width, int _height, Drawer* _drawer);
    void removeShape(Shape* _shape);
    void draw() const;

    Shape* getSelectedShapePriority(int _xpos, int _ypos, Shape* _prioShape, bool& _isPrio);
    Shape* getSelectedShape(int _xpos, int _ypos) const;
    vector<int> getIndex(Shape* _shape);

    vector<Shape>& getShapeVector();

    size_t getShapeCount() const;
};

#endif //OPEN_SKETCH_GROUP_H
