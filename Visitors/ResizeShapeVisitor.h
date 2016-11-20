//
// Created by hindrik on 16-11-16.
//

#ifndef OPEN_SKETCH_RESIZESHAPEVISITOR_H
#define OPEN_SKETCH_RESIZESHAPEVISITOR_H

#include "../Shapes/Visitor.h"
#include <vector>
#include <string>
#include <stack>

using std::stack;
using std::vector;
using std::string;

struct ResizeData {
    int m_pixelsX = 0;
    int m_pixelsY = 0;
    int m_old_pos_x = 0;
    int m_old_pos_y = 0;
    int m_old_width = 0;
    int m_old_height = 0;
    float m_factorX = 0.0f;
    float m_factorY = 0.0f;
};

class ResizeShapeVisitor : public Visitor {
private:
    int m_pixelsX = 0;
    int m_pixelsY = 0;
    stack<ResizeData> m_stack;
public:
    ResizeShapeVisitor(int _pixelsX, int _pixelsY);

    virtual void start_visit(Shape &_shape) override;

    virtual void start_visit(Group &_group) override;

    virtual void stop_visit(Shape &_shape) override;

    virtual void stop_visit(Group &_group) override;
};


#endif //OPEN_SKETCH_RESIZESHAPEVISITOR_H
