//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "../OpenGL/OpenGLRenderManager.h"
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

void selectedit(GtkWidget *widget, gpointer data)
{
    Application::getInstance().setEditMode(EditMode::Select_and_edit);
}

void setEdited(GtkWidget *widget, gpointer data)
{
    Application::getInstance().setShapeEdited(true);
}

void addRect(GtkWidget *widget, gpointer data)
{
    Application::getInstance().getGLManager().addRectangle(100,100,100,100);
}

void addEllips(GtkWidget *widget, gpointer data)
{
    Application::getInstance().getGLManager().addEllipse(100,100,100,100);
}

void deleteShape(GtkWidget* widget, gpointer data)
{
    Application::getInstance().setShapeDeleted(true);
}