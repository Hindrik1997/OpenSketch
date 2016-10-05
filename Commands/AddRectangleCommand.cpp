//
// Created by hindrik on 4-10-16.
//

#include "AddRectangleCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

using namespace std;

void AddRectangleCommand::execute(Application *_context)
{
    _context->getGLManager().addRectangle(m_posx, m_posy, m_sizex, m_sizey);
}

void AddRectangleCommand::undo(Application *_context)
{
    _context->getGLManager().removeLastRectangle();
}

AddRectangleCommand::AddRectangleCommand(int _posx, int _posy, int _sizex, int _sizey) : m_sizex(_sizex), m_sizey(_sizey), m_posx(_posx), m_posy(_posy)
{
}
