//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_ADDELLIPSECOMMAND_H
#define OPEN_SKETCH_ADDELLIPSECOMMAND_H

#include "Command.h"


class Application;

class AddEllipseCommand : public Command<Application> {
private:
    int m_posx = 100;
    int m_posy = 100;
    int m_sizex = 100;
    int m_sizey = 100;
public:
    AddEllipseCommand() = default;
    AddEllipseCommand(int _posx, int _posy, int _sizex, int _sizey);

    void execute(Application* _context);
    void undo(Application* _context);
};

#endif //OPEN_SKETCH_ADDELLIPSECOMMAND_H
