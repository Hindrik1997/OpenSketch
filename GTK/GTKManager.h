//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_GTKMANAGER_H
#define OPEN_SKETCH_GTKMANAGER_H

#include "../includeLibs.h"
#include "GTKWindow.h"
#include <vector>
#include <string>

using std::string;

class GTKManager {
private:
    std::vector<GTKWindow> m_windows;
public:
    GTKManager(int argc, char* argv[]);
    ~GTKManager();
public:
    bool updateAPI();
    GTKWindow& createWindow(int width, int height, string title);

};


#endif //OPEN_SKETCH_GTKMANAGER_H
