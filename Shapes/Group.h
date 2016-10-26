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

    void draw() const override;

    virtual void setPosition(int _pixelsX, int _pixelsY) override;

    virtual void setSize(int _width, int _height) override;

    virtual glm::mat4 getMatrix() const override;

    virtual glm::vec2 getPosition() const override;

    virtual glm::vec2 getSize() const override;


};

#endif //OPEN_SKETCH_GROUP_H
