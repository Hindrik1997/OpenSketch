//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_GTKCALLBACKS_H
#define OPEN_SKETCH_GTKCALLBACKS_H

#include "../includeLibs.h"

void clicked(GtkWidget* widget, gpointer data);

void suppress(GtkWidget* widget, gpointer data);

void selectmove(GtkWidget* widget, gpointer data);

void noeditmode(GtkWidget* widget, gpointer data);

void selectedit(GtkWidget* widget, gpointer data);

void setEdited(GtkWidget* widget, gpointer data);

void addRect(GtkWidget* widget, gpointer data);

void addEllips(GtkWidget* widget, gpointer data);

void deleteShape(GtkWidget* widget, gpointer data);

void redoCommand(GtkWidget* widget, gpointer data);

void undoCommand(GtkWidget* widget, gpointer data);

#endif //OPEN_SKETCH_GTKCALLBACKS_H
