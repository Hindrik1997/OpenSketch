//
// Created by hindrik on 5-10-16.
//

#include "AddEllipseCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

void AddEllipseCommand::execute(Application *_context)
{
    _context->getGLManager().addEllipse(m_posx, m_posy, m_sizex, m_sizey);
}

void AddEllipseCommand::undo(Application *_context)
{
    _context->getGLManager().removeLastEllipse();
}

AddEllipseCommand::AddEllipseCommand(int _posx, int _posy, int _sizex, int _sizey) : m_sizex(_sizex), m_sizey(_sizey), m_posx(_posx), m_posy(_posy)
{
}