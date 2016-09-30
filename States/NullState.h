//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_NULLSTATE_H
#define OPEN_SKETCH_NULLSTATE_H


#include "State.h"

class NullState : public State {
public:
    void doAction(Application* _context);
};


#endif //OPEN_SKETCH_NULLSTATE_H
