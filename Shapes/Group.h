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

class Group : public Shape, public Visitable<Group> {
private:
    vector<unique_ptr<Shape>> m_shapes;
public:
    Group(ShapeRenderManager* _oglRenderer, vector<unique_ptr<Shape>>& _vec);
    virtual ~Group() override = default;

    virtual vector<string> writeToFile() override;

    virtual void accept(Visitor<Group> &_v) override;

    virtual void setPosition(int _pixelsX, int _pixelsY) override;

    virtual void setSize(int _width, int _height) override;

    virtual glm::vec2 getPosition() const override;

    virtual glm::vec2 getSize() const override;

    virtual bool getSelected() const override;

    vector<unique_ptr<Shape>>& getShapes();
private:
    void calculateMetrics(int& _minx, int& _maxx, int& _miny, int& _maxy) const;
};

#endif //OPEN_SKETCH_GROUP_H
