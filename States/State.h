//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_STATE_H
#define OPEN_SKETCH_STATE_H

class Application;

class State {
public:
    virtual void doAction(Application* _context) = 0;
};


#endif //OPEN_SKETCH_STATE_H
