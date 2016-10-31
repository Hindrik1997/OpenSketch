//
// Created by hindrik on 5-10-16.
//

#ifndef OPEN_SKETCH_REMOVEELLIPSECOMMAND_H
#define OPEN_SKETCH_REMOVEELLIPSECOMMAND_H

#include <memory>
#include <vector>
#include <cstddef>
#include "Command.h"
#include "../Shapes/Shape.h"

using std::unique_ptr;

class Drawer;
class Application;

class RemoveShapeCommand : public Command<Application>
{
private:
    size_t m_index;
    unique_ptr<Shape> m_data = nullptr;
public:
    RemoveShapeCommand(size_t _index);

    void execute(Application* _context) override;
    void undo(Application* _context) override;
};


#endif //OPEN_SKETCH_REMOVEELLIPSECOMMAND_H
