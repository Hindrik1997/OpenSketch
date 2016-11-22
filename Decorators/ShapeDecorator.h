//
// Created by Hindrik Stegenga on 20-11-16.
//

#ifndef OPEN_SKETCH_SHAPEDECORATOR_H
#define OPEN_SKETCH_SHAPEDECORATOR_H

#include "../Shapes/Shape.h"

class ShapeDecorator : public Shape {
public:
    Shape* m_shape;
public:
    ShapeDecorator(Shape* _ishape);

    virtual void draw() const override;

    virtual void resize(int _pixelsX, int _pixelsY) override;

    virtual void move(int _pixelsX, int _pixelsY) override;

    virtual void setSelected(bool _isSelected) override;

    virtual bool getSelected() const override;

    virtual glm::mat4 getMatrix() const override;

    virtual void accept(Visitor &_v) override;

    virtual glm::vec2 getPosition() const override;

    virtual glm::vec2 getSize() const override;

    virtual Drawer *getDrawer() const override;

    inline virtual ~ShapeDecorator() = 0;
};

ShapeDecorator::~ShapeDecorator(){}

#endif //OPEN_SKETCH_SHAPEDECORATOR_H
