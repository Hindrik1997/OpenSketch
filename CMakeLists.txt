#[[
    De volgende libs zijn nodig:

    GLFW3
    GLEW
    GTK+ 2.0 en de dev libs

    Zie ook: http://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project
    Ik heb zelf GLFW3 en GLEW sources gedownload en gecompiled en geinstalled zoals hierboven aangegeven.

    De library GLM zit ook in het project, alleen deze is header only dus dit zit onder de map /GLM

    copy paste deze commands in de terminal voor OpenGL dev packages:

    sudo apt-get install cmake xorg-dev libglu1-mesa-dev

    mogelijk is ook gtk2.0 nodig.

    sudo apt-get install gtk+-2.0

]]

cmake_minimum_required(VERSION 3.2.2)
project(open_sketch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include(FindGTK2)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
    link_directories(${GTK2_LIBRARY_DIRS})
    link_libraries(${GTK2_LIBRARIES})
endif (GTK2_FOUND)

link_libraries(libGLEW.a glfw3.a GL GLU X11 Xxf86vm dl Xinerama Xcursor pthread Xi Xrandr)

set(SOURCE_FILES main.cpp main.cpp GLFW/GLFWfunctions.cpp GLFW/GLFWfunctions.h Application.cpp Application.h GTK/gtkCallbacks.cpp GTK/gtkCallbacks.h includeLibs.h GTK/GTKManager.cpp GTK/GTKManager.h GTK/GTKWindow.cpp GTK/GTKWindow.h OpenGL/OpenGLRenderManager.cpp OpenGL/OpenGLRenderManager.h OpenGL/DrawableObject.cpp OpenGL/DrawableObject.h Shapes/Rectangle.cpp Shapes/Rectangle.h Shapes/Ellipse.cpp Shapes/Ellipse.h States/State.h SupportClasses.h States/EditShapeState.cpp States/EditShapeState.h States/MoveShapeState.cpp States/MoveShapeState.h States/NullState.cpp States/NullState.h Commands/Command.cpp Commands/Command.h Commands/CommandContext.cpp Commands/CommandContext.h Shapes/Shape.cpp Shapes/Shape.h Commands/AddRectangleCommand.cpp Commands/AddRectangleCommand.h)
add_executable(open_sketch ${SOURCE_FILES})

add_custom_command(TARGET open_sketch PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:open_sketch>)