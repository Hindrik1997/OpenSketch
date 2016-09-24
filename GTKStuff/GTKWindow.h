//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_GTKWINDOW_H
#define OPEN_SKETCH_GTKWINDOW_H

#include "../includeLibs.h"
#include <string>

using std::string;

class GTKManager;

class GTKWindow {
private:
    static int m_globalIDcounter;

    int m_id;
    GTKManager& m_Manager;

    string m_title;
    int m_width;
    int m_height;

    GtkWidget* m_pWindow;
public:
    GTKWindow(GTKManager& _manager, int _width, int _height, string _title);

    int getID();

    void present();
    void show();
    void hide();


};


#endif //OPEN_SKETCH_GTKWINDOW_H
