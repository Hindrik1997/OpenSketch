//
// Created by Hindrik Stegenga on 31-10-16.
//

#ifndef OPEN_SKETCH_UNFORMGROUPCOMMAND_H
#define OPEN_SKETCH_UNFORMGROUPCOMMAND_H

#include <algorithm>
#include <memory>
#include <vector>
#include <cstddef>
#include "Command.h"
#include "../Shapes/Shape.h"

class Drawer;
class Application;

using std::unique_ptr;

/*
 * Command to unform groups
 * */
class UnFormGroupCommand : public Command<Application> {
private:
    size_t m_groupSize;
    size_t m_groupIndex;
    unique_ptr<Shape> m_group;
public:
    UnFormGroupCommand(size_t _index);

    virtual void execute(Application *_context) override;

    virtual void undo(Application *_context) override;

};


#endif //OPEN_SKETCH_UNFORMGROUPCOMMAND_H
