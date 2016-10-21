//
// Created by hindrik on 5-10-16.
//

#ifndef OPEN_SKETCH_CHANGEELLIPSECOMMAND_H
#define OPEN_SKETCH_CHANGEELLIPSECOMMAND_H


#include <cstddef>
#include "Command.h"

class Application;

class ChangeShapeCommand : public Command<Application>
{
private:
    std::size_t m_index;
    int m_posx_change;
    int m_posy_change;
    int m_sizex_change;
    int m_sizey_change;
public:
    ChangeShapeCommand(std::size_t _index, int _posx_change, int _posy_change, int _sizex_change, int _sizey_change);

    void execute(Application* _context);
    void undo(Application* _context);

};


#endif //OPEN_SKETCH_CHANGEELLIPSECOMMAND_H
