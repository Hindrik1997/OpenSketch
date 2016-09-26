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

set(SOURCE_FILES main.cpp main.cpp GLFW/GLFWfunctions.cpp GLFW/GLFWfunctions.h Application.cpp Application.h GTK/gtkCallbacks.cpp GTK/gtkCallbacks.h includeLibs.h GTK/GTKManager.cpp GTK/GTKManager.h GTK/GTKWindow.cpp GTK/GTKWindow.h OpenGL/OpenGLRenderManager.cpp OpenGL/OpenGLRenderManager.h OpenGL/DrawableObject.cpp OpenGL/DrawableObject.h)
add_executable(open_sketch ${SOURCE_FILES})

add_custom_command(TARGET open_sketch PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:open_sketch>)