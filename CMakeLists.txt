cmake_minimum_required(VERSION 3.2.2)
project(open_sketch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include(FindGTK2)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
    link_directories(${GTK2_LIBRARY_DIRS})
    link_libraries(${GTK2_LIBRARIES})
endif (GTK2_FOUND)

link_libraries(libGLEW.a glfw3 GL GLU X11 Xxf86vm dl Xinerama Xcursor pthread Xi Xrandr)

set(SOURCE_FILES main.cpp main.cpp functions.cpp functions.h Application.cpp Application.h GTKStuff/gtkCallbacks.cpp GTKStuff/gtkCallbacks.h includeLibs.h GTKStuff/GTKManager.cpp GTKStuff/GTKManager.h GTKStuff/GTKWindow.cpp GTKStuff/GTKWindow.h)
add_executable(open_sketch ${SOURCE_FILES})
