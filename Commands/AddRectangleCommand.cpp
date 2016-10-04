//
// Created by hindrik on 4-10-16.
//

#include "AddRectangleCommand.h"
#include <iostream>

using namespace std;

void AddRectangleCommand::execute(Application *_context) {
    cout << "Hello from execute()" << endl;
}

void AddRectangleCommand::undo(Application *_context) {
    cout << "Hello from undo()" << endl;
}
