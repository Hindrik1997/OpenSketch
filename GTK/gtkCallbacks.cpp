//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "../States/NullState.h"
#include "../States/MoveShapeState.h"
#include "../States/EditShapeState.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "gtkCallbacks.h"
#include "../Commands/AddRectangleCommand.h"
#include "../Commands/AddEllipseCommand.h"
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
    Application::getInstance().execute(new AddRectangleCommand());
}

void addEllips(GtkWidget *widget, gpointer data)
{
    Application::getInstance().execute(new AddEllipseCommand());
}

void deleteShape(GtkWidget* widget, gpointer data)
{
    Application::getInstance().setShapeDeleted(true);
}

void redoCommand(GtkWidget *widget, gpointer data) {
    Application::getInstance().redo();
}

void undoCommand(GtkWidget *widget, gpointer data) {
    Application::getInstance().undo();
}

void loadButton(GtkWidget *widget, gpointer data) {
    Application::getInstance().loadFromFile();
}

void saveButton(GtkWidget *widget, gpointer data) {
    Application::getInstance().saveToFile();
}
