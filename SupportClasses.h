//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_SUPPORTCLASSES_H
#define OPEN_SKETCH_SUPPORTCLASSES_H

//Args struct
struct Args
{
    int m_argc = 0;
    char** m_argv = nullptr;
};

//ShapeType
enum class shapeType : char
{
    NullType,
    EllipseType,
    RectangleType,
};

//ShapeInfo
struct shapeInfo {
    void* m_shapePointer = nullptr;
    shapeType m_shapeT;
};

#endif //OPEN_SKETCH_SUPPORTCLASSES_H
