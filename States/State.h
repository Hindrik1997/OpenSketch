//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_STATE_H
#define OPEN_SKETCH_STATE_H

/*
 *
 * STATE PATTERN
 *
 * Deze class implement een state pattern, zodat ik gemakkelijk verschillende states kan toevoegen aan mn applicatie.
 * Dit is veel flexibeler dan een switch of iets dergelijks.
 * Zie ook Application.h en Application.cpp
 * */


class Application;

class State {
public:
    virtual void doAction(Application* _context) = 0;
};


#endif //OPEN_SKETCH_STATE_H
