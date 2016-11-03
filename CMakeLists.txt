#[[
    De volgende libs zijn nodig:

    GLFW3
    GLEW
    GTK+ 2.0 en de dev libs
    FreeType 2.7

    Zie ook: http://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project
    Ik heb zelf GLFW3, GLEW en FreeType sources gedownload en gecompiled en geinstalled zoals hierboven aangegeven of via make + make install

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

link_libraries(libGLEW.a glfw3.a GL GLU X11 Xxf86vm dl Xinerama Xcursor pthread Xi Xrandr freetype)

set(SOURCE_FILES main.cpp main.cpp GLFW/GLFWfunctions.cpp GLFW/GLFWfunctions.h Application.cpp Application.h GTK/gtkCallbacks.cpp GTK/gtkCallbacks.h includeLibs.h GTK/GTKManager.cpp GTK/GTKManager.h GTK/GTKWindow.cpp GTK/GTKWindow.h OpenGL/ShapeRenderManager.cpp OpenGL/ShapeRenderManager.h OpenGL/DrawableObject.cpp OpenGL/DrawableObject.h States/State.h SupportClasses.h States/EditShapeState.cpp States/EditShapeState.h States/MoveShapeState.cpp States/MoveShapeState.h States/NullState.cpp States/NullState.h Commands/Command.h Commands/CommandContext.h Shapes/Shape.cpp Shapes/Shape.h Commands/AddShapeCommand.cpp Commands/AddShapeCommand.h Commands/ChangeShapeCommand.cpp Commands/ChangeShapeCommand.h Commands/RemoveShapeCommand.cpp Commands/RemoveShapeCommand.h OpenGL/Drawer.h OpenGL/Drawer.cpp OpenGL/RectangleDrawer.h OpenGL/RectangleDrawer.cpp OpenGL/EllipseDrawer.h OpenGL/EllipseDrawer.cpp Shapes/Group.cpp Shapes/Group.h States/GroupShapesState.cpp States/GroupShapesState.h Commands/FormGroupCommand.cpp Commands/FormGroupCommand.h Commands/UnFormGroupCommand.cpp Commands/UnFormGroupCommand.h)
add_executable(open_sketch ${SOURCE_FILES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(open_sketch PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(open_sketch PUBLIC DEBUG=0)
endif()

add_custom_command(TARGET open_sketch PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:open_sketch>)
add_custom_command(TARGET open_sketch PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data $<TARGET_FILE_DIR:open_sketch>)
add_custom_command(TARGET open_sketch PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Fonts $<TARGET_FILE_DIR:open_sketch>)