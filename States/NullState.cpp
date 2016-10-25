//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "NullState.h"
#include "../Application.h"

void NullState::doAction(Application *_context) {
    //Do absolutely nothing at all!
    //Okay, a bit maybe!
    Application::getInstance().resetState();
}
