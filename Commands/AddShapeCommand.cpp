//
// Created by hindrik on 5-10-16.
//

#include "AddShapeCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

void AddShapeCommand::execute(Application *_context)
{
    _context->getGLManager().addShape(m_posx, m_posy, m_sizex, m_sizey, m_drawer);
}

void AddShapeCommand::undo(Application *_context)
{
    _context->getGLManager().removeLastShape();
}

AddShapeCommand::AddShapeCommand(int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer) : m_sizex(_sizex), m_sizey(_sizey), m_posx(_posx), m_posy(_posy), m_drawer(_drawer)
{
}