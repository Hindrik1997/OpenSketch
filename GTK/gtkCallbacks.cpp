//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "../States/NullState.h"
#include "../States/MoveShapeState.h"
#include "../States/EditShapeState.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "gtkCallbacks.h"
#include "../Commands/AddShapeCommand.h"
#include "../States/GroupShapesState.h"
#include "../OpenGL/RectangleDrawer.h"
#include "../OpenGL/EllipseDrawer.h"
#include "../Commands/FormGroupCommand.h"
#include "../Commands/AddDecoratorCommand.h"

static NullState nullState;
static MoveShapeState moveState;
static EditShapeState editState;
static GroupShapesState groupState;

string trim(string& str);

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
    if(vec.size() == 0) return;
    Application::getInstance().execute(new FormGroupCommand(vec));
    Application::getInstance().resetState();
    Application::getInstance().setState(&nullState);
}

void ungroupButton(GtkWidget *widget, gpointer data) {
    Application::getInstance().setGroupDeformed(true);
}

void selectgroupButton(GtkWidget *widget, gpointer data) {
    Application::getInstance().setState(&groupState);
}

void add_ornament_top_command(GtkWidget *widget, gpointer data) {
    Shape* selected = Application::getInstance().getM_selectedShape();
    if(selected == nullptr)
    {
        return;
    }
    string text (gtk_entry_get_text(GTK_ENTRY(Application::getInstance().getM_ornament_field())));
    if(text.size() != 0)
    {
        text = trim(text);
        if(text.size() == 0)
            return;
    }
    Application::getInstance().execute(new AddDecoratorCommand(
            Application::getInstance().getGLManager().getIndex(selected)
            ,text,"top"));
    Application::getInstance().resetState();
}

void add_ornament_bottom_command(GtkWidget *widget, gpointer data) {
    Shape* selected = Application::getInstance().getM_selectedShape();
    if(selected == nullptr)
    {
        return;
    }
    string text (gtk_entry_get_text(GTK_ENTRY(Application::getInstance().getM_ornament_field())));
    if(text.size() != 0)
    {
        text = trim(text);
        if(text.size() == 0)
            return;
    }
    Application::getInstance().execute(new AddDecoratorCommand(
            Application::getInstance().getGLManager().getIndex(selected)
            ,text,"bottom"));
    Application::getInstance().resetState();
}

void add_ornament_left_command(GtkWidget *widget, gpointer data) {
    Shape* selected = Application::getInstance().getM_selectedShape();
    if(selected == nullptr)
    {
        return;
    }
    string text (gtk_entry_get_text(GTK_ENTRY(Application::getInstance().getM_ornament_field())));
    if(text.size() != 0)
    {
        text = trim(text);
        if(text.size() == 0)
            return;
    }
    Application::getInstance().execute(new AddDecoratorCommand(
            Application::getInstance().getGLManager().getIndex(selected)
            ,text,"left"));
    Application::getInstance().resetState();
}

void add_ornament_right_command(GtkWidget *widget, gpointer data) {
    Shape* selected = Application::getInstance().getM_selectedShape();
    if(selected == nullptr)
    {
        return;
    }
    string text (gtk_entry_get_text(GTK_ENTRY(Application::getInstance().getM_ornament_field())));
    if(text.size() != 0)
    {
        text = trim(text);
        if(text.size() == 0)
            return;
    }
    Application::getInstance().execute(new AddDecoratorCommand(
            Application::getInstance().getGLManager().getIndex(selected)
            ,text,"right"));
    Application::getInstance().resetState();
}