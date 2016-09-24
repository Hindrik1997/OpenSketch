cmake_minimum_required(VERSION 3.2.2)
project(open_sketch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

link_libraries(libGLEW.a glfw3 GL GLU X11 Xxf86vm dl Xinerama Xcursor pthread Xi Xrandr)

set(SOURCE_FILES main.cpp main.cpp functions.cpp functions.h)
add_executable(open_sketch ${SOURCE_FILES})
