//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "gtkCallbacks.h"
#include "../Application.h"

void clicked(GtkWidget *widget, gpointer data) {
    //er is geklikt

}

void suppress(GtkWidget *widget, gpointer data)
{
    //doe niks
}

void selectmove(GtkWidget* widget, gpointer data)
{
    Application::getInstance().setEditMode(EditMode::Select_and_move);
}

void noeditmode(GtkWidget *widget, gpointer data)
{
    Application::getInstance().setEditMode(EditMode::Null_mode);
}
