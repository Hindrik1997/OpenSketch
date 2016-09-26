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
    return true;
}

GTKWindow& GTKManager::createWindow(int width, int height, string title) {
    m_windows.push_back(std::move(GTKWindow(*this, width, height, title)));
    return m_windows.back();
}


