//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "Application.h"

Application &Application::getInstance() {
    static Application instance;
    return instance;
}

Application::Application() {
}
