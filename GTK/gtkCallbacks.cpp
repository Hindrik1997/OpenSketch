//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "../States/NullState.h"
#include "../States/MoveShapeState.h"
#include "../States/EditShapeState.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "gtkCallbacks.h"
#include "../Commands/AddShapeCommand.h"
#include "../States/GroupShapesState.h"
#include "../OpenGL/RectangleDrawer.h"
#include "../OpenGL/EllipseDrawer.h"
#include "../Commands/FormGroupCommand.h"

static NullState nullState;
static MoveShapeState moveState;
static EditShapeState editState;
static GroupShapesState groupState;


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
    Application::getInstance().execute(new AddShapeCommand(100,100,100,100, &RectangleDrawer::getInstance()));
    Application::getInstance().setState(&nullState);
}

void addEllips(GtkWidget *widget, gpointer data)
{
    Application::getInstance().execute(new AddShapeCommand(100,100,100,100, &EllipseDrawer::getInstance()));
    Application::getInstance().setState(&nullState);
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

void groupButton(GtkWidget *widget, gpointer data) {
    auto& vec = Application::getInstance().getSelectedShapes();
    Application::getInstance().execute(new FormGroupCommand(vec));
    Application::getInstance().resetState();
    Application::getInstance().setState(&nullState);
}

void ungroupButton(GtkWidget *widget, gpointer data) {

}

void selectgroupButton(GtkWidget *widget, gpointer data) {
    Application::getInstance().setState(&groupState);
}
