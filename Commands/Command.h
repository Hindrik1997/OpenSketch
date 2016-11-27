//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_COMMAND_H
#define OPEN_SKETCH_COMMAND_H


/*
 * COMMAND PATTERN
 *
 * Command base class. Implements a generic interface to implement commands
 *
 *
 *
 * */


template<typename D>
class CommandContext;

template<typename T>
class Command {
public:
    virtual void execute(T *_context) = 0;
    virtual void undo(T *_context) = 0;
};


#endif //OPEN_SKETCH_COMMAND_H