//
// Created by hindrik on 4-10-16.
//

#ifndef OPEN_SKETCH_ADDRECTANGLECOMMAND_H
#define OPEN_SKETCH_ADDRECTANGLECOMMAND_H

#include "Command.h"


class Application;

class AddRectangleCommand : public Command<Application> {
public:
    void execute(Application* _context);
    void undo(Application* _context);

};


#endif //OPEN_SKETCH_ADDRECTANGLECOMMAND_H
