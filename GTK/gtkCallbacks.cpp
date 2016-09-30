//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "../States/NullState.h"
#include "../States/MoveShapeState.h"
#include "../States/EditShapeState.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "gtkCallbacks.h"
#include "../Application.h"

static NullState nullState;
static MoveShapeState moveState;
static EditShapeState editState;


void clicked(GtkWidget *widget, gpointer data) {
    //er is geklikt

}

void suppress(GtkWidget *widget, gpointer data)
{
    //doe niks
}

void selectmove(GtkWidget* widget, gpointer data)
{
    Application::getInstance().setState(&moveState);
}

void noeditmode(GtkWidget *widget, gpointer data)
{
    Application::getInstance().setState(&nullState);
}

void selectedit(GtkWidget *widget, gpointer data)
{
    Application::getInstance().setState(&editState);
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
    Application::getInstance().getGLManager().addEllipse(100,100,200,100);
}

void deleteShape(GtkWidget* widget, gpointer data)
{
    Application::getInstance().setShapeDeleted(true);
}