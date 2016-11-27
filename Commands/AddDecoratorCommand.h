//
// Created by Hindrik Stegenga on 23-11-16.
//

#ifndef OPEN_SKETCH_ADDDECORATORCOMMAND_H
#define OPEN_SKETCH_ADDDECORATORCOMMAND_H

#include <string>
#include <vector>
#include <cstddef>
#include "Command.h"

using std::string;

class Drawer;
class Application;


/*
 * Implements the addition of a decorator to a shape.
 * */
class AddDecoratorCommand : public Command<Application> {
private:
    size_t m_index;
    string m_text;
    string m_type;
public:
    AddDecoratorCommand(size_t _index, string _text, string _type);

    void execute(Application *_context) override;

    void undo(Application *_context) override;

};


#endif //OPEN_SKETCH_ADDDECORATORCOMMAND_H
