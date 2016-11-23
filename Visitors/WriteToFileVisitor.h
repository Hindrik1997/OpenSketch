//
// Created by Hindrik Stegenga on 9-11-16.
//

#ifndef OPEN_SKETCH_WRITETOFILEVISITOR_H
#define OPEN_SKETCH_WRITETOFILEVISITOR_H


#include <string>
#include <vector>
#include "../Shapes/Visitor.h"
#include "../Decorators/OrnamentBottomDecorator.h"

using namespace std;

/*
 * Visitor die file writing implementeert.
 * */


class WriteToFileVisitor : public Visitor {
private:
    vector<string> m_data;
    int m_tabs = 0;
public:
    virtual void start_visit(Shape &_shape) override;

    virtual void start_visit(Group &_group) override;

    vector<string> getData();

    virtual void stop_visit(Shape &_shape) override;

    virtual void stop_visit(Group &_group) override;
};

#endif //OPEN_SKETCH_WRITETOFILEVISITOR_H
