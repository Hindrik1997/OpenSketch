//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_ADDRECTANGLECOMMAND_H
#define OPEN_SKETCH_ADDRECTANGLECOMMAND_H

#include "Command.h"


class Application;

class AddRectangleCommand : public Command<Application> {
private:
    int m_posx = 100;
    int m_posy = 100;
    int m_sizex = 100;
    int m_sizey = 100;
public:
    AddRectangleCommand() = default;
    AddRectangleCommand(int _posx, int _posy, int _sizex, int _sizey);

    void execute(Application* _context);
    void undo(Application* _context);
};

#endif //OPEN_SKETCH_ADDRECTANGLECOMMAND_H
