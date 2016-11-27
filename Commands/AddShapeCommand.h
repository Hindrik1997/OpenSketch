//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_ADDELLIPSECOMMAND_H
#define OPEN_SKETCH_ADDELLIPSECOMMAND_H

#include <vector>
#include <cstddef>
#include "Command.h"

class Drawer;
class Application;


/*
 * Implements the addition of a shape to the application
 * */
class AddShapeCommand : public Command<Application> {
private:
    int m_posx = 100;
    int m_posy = 100;
    int m_sizex = 100;
    int m_sizey = 100;
    Drawer* m_drawer = nullptr;
    size_t m_index;
public:
    AddShapeCommand(int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer);

    void execute(Application* _context) override;
    void undo(Application* _context) override;
};

#endif //OPEN_SKETCH_ADDELLIPSECOMMAND_H
