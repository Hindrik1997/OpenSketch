//
// Created by hindrik on 5-10-16.
//

#ifndef OPEN_SKETCH_REMOVEELLIPSECOMMAND_H
#define OPEN_SKETCH_REMOVEELLIPSECOMMAND_H

#include <cstddef>
#include "Command.h"

class Drawer;
class Application;

class RemoveShapeCommand : public Command<Application>
{
private:
    size_t m_index;
    int m_posx = 100;
    int m_posy = 100;
    int m_sizex = 100;
    int m_sizey = 100;
    Drawer* m_drawer = nullptr;
public:
    RemoveShapeCommand(size_t _index, int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer);

    void execute(Application* _context);
    void undo(Application* _context);

};


#endif //OPEN_SKETCH_REMOVEELLIPSECOMMAND_H
