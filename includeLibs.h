//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_LIBSINCLUDE_H
#define OPEN_SKETCH_LIBSINCLUDE_H

// GLEW as static lib
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//GTK2
#include <gtk/gtk.h>

#define SAFE_PNTR_DEL(x) if(x != nullptr) delete x;

#endif //OPEN_SKETCH_LIBSINCLUDE_H
