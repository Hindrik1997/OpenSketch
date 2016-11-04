//
// Created by hindrik on 5-10-16.
//

#include <vector>
#include "AddShapeCommand.h"
#include "../OpenGL/ShapeRenderManager.h"

using std::vector;

void AddShapeCommand::execute(Application *_context)
{
    Shape& s = _context->getGLManager().addShape(m_posx, m_posy, m_sizex, m_sizey, m_drawer);
    m_index = _context->getGLManager().getIndex(&s);
}

void AddShapeCommand::undo(Application *_context)
{
    _context->getGLManager().getShapes().erase(_context->getGLManager().getShapes().begin() + m_index);
}

AddShapeCommand::AddShapeCommand(int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer) : m_sizex(_sizex), m_sizey(_sizey), m_posx(_posx), m_posy(_posy), m_drawer(_drawer)
{
}