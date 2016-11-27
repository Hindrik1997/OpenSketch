//
// Created by Hindrik Stegenga on 27-10-16.
//

#ifndef OPEN_SKETCH_FORMGROUPCOMMAND_H
#define OPEN_SKETCH_FORMGROUPCOMMAND_H

#include <memory>
#include <vector>
#include <cstddef>
#include "Command.h"
#include "../Shapes/Shape.h"

class Drawer;
class Application;

using std::vector;
using std::unique_ptr;

/*
 * Implements the command to form groups of shapes
 * */
class FormGroupCommand : public Command<Application> {
private:
    vector<size_t> m_indices;
    size_t m_groupIndex;
public:
    FormGroupCommand(vector<size_t> _indices);


    virtual void execute(Application *_context) override;

    virtual void undo(Application *_context) override;

};


#endif //OPEN_SKETCH_FORMGROUPCOMMAND_H
