//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "Application.h"


int main(int argc, char* argv[])
{
    Args args {argc, argv};
    Application& application = Application::getInstance();
    application.setStartupArgs(args);
    application.run();
    return 0;
}