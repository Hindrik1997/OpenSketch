//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "GTKManager.h"

GTKManager::GTKManager(int argc, char* argv[]) {
    gtk_init(&argc, &argv);
}

GTKManager::~GTKManager() {
}

bool GTKManager::updateAPI() {

    if(gtk_events_pending())
    {
        gtk_main_iteration();
    }
}

GTKWindow& GTKManager::createWindow(int width, int height, string title) {
    windows.push_back(GTKWindow(*this, width, height, title));
    return windows.back();
}


