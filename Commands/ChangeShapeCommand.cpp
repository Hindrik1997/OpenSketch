//
// Created by hindrik on 5-10-16.
//

#include <algorithm>
#include "ChangeShapeCommand.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../Visitors/MoveShapeVisitor.h"
#include "../Visitors/ResizeShapeVisitor.h"

ChangeShapeCommand::ChangeShapeCommand(size_t _index,int _posx_change, int _posy_change, int _sizex_change, int _sizey_change)
        : m_posx_change(_posx_change), m_posy_change(_posy_change), m_sizex_change(_sizex_change), m_sizey_change(_sizey_change), m_index(_index)
{
}

void ChangeShapeCommand::execute(Application *_context)
{
    Shape& rect = *_context->getGLManager().getShapes()[m_index];
    MoveShapeVisitor v(m_posx_change, m_posy_change);
    rect.accept(v);
    ResizeShapeVisitor v2(m_sizex_change, m_sizey_change);
    rect.accept(v2);
}

void ChangeShapeCommand::undo(Application *_context)
{
    Shape& rect = *_context->getGLManager().getShapes()[m_index];
    MoveShapeVisitor v(-m_posx_change, -m_posy_change);
    rect.accept(v);
    ResizeShapeVisitor v2(-m_sizex_change, -m_sizey_change);
    rect.accept(v2);
}