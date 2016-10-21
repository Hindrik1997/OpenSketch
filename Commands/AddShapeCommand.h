//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_ADDELLIPSECOMMAND_H
#define OPEN_SKETCH_ADDELLIPSECOMMAND_H

#include "Command.h"

class Drawer;
class Application;

class AddShapeCommand : public Command<Application> {
private:
    int m_posx = 100;
    int m_posy = 100;
    int m_sizex = 100;
    int m_sizey = 100;
    Drawer* m_drawer = nullptr;
public:
    AddShapeCommand(int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer);

    void execute(Application* _context);
    void undo(Application* _context);
};

#endif //OPEN_SKETCH_ADDELLIPSECOMMAND_H
