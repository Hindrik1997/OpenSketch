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
    Group(vector<unique_ptr<Shape>>& _vec);
    virtual ~Group() override = default;

    virtual void accept(Visitor &_v) override;

    virtual glm::vec2 getPosition() const override;
    virtual glm::vec2 getSize() const override;

    vector<unique_ptr<Shape>>& getShapes();
private:
    void calculateMetrics(int& _minx, int& _maxx, int& _miny, int& _maxy) const;
};

#endif //OPEN_SKETCH_GROUP_Hdddddd
