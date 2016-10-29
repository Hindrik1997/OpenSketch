//
// Created by Hindrik Stegenga on 21-10-16.
//

#ifndef OPEN_SKETCH_GROUP_H
#define OPEN_SKETCH_GROUP_H

#include <memory>
#include <typeinfo>
#include <vector>
#include "Shape.h"

using std::vector;
using std::unique_ptr;

class Group : public Shape {
private:
    vector<unique_ptr<Shape>> m_shapes;
public:
    Group(OpenGLRenderManager* _oglRenderer, vector<unique_ptr<Shape>>& _vec);
    virtual ~Group() override = default;


    void draw() const override;

    virtual void setPosition(int _pixelsX, int _pixelsY) override;

    virtual void setSize(int _width, int _height) override;

    virtual glm::mat4 getMatrix() const override;

    virtual glm::vec2 getPosition() const override;

    virtual glm::vec2 getSize() const override;

    vector<unique_ptr<Shape>>& getShapes();
};

#endif //OPEN_SKETCH_GROUP_H
