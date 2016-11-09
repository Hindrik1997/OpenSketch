//
// Created by Hindrik Stegenga on 9-11-16.
//

#ifndef OPEN_SKETCH_WRITETOFILEVISITOR_H
#define OPEN_SKETCH_WRITETOFILEVISITOR_H


#include <string>
#include <vector>
#include "../Shapes/Visitor.h"

using namespace std;

class WriteToFileVisitor : public Visitor {
private:
    vector<string> m_data;
public:
    virtual void visit(Shape &_shape) override;

    virtual void visit(Group &_group) override;

    vector<string> getData();
};


#endif //OPEN_SKETCH_WRITETOFILEVISITOR_H
