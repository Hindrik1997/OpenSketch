//
// Created by hindrik on 5-10-16.
//

#include "RemoveEllipseCommand.h"
#include "../Shapes/Ellipse.h"
#include "../OpenGL/OpenGLRenderManager.h"

RemoveEllipseCommand::RemoveEllipseCommand(size_t _index, int _posx, int _posy, int _sizex, int _sizey)
        : m_index(_index), m_posx(_posx), m_posy(_posy), m_sizex(_sizex), m_sizey(_sizey)
{
}

void RemoveEllipseCommand::execute(Application *_context)
{
    vector<Ellipse>& vec = const_cast<vector<Ellipse>&>(_context->getGLManager().getEllipses());
    vec.erase(vec.begin() + m_index);
}

void RemoveEllipseCommand::undo(Application *_context)
{
    vector<Ellipse>& vec = const_cast<vector<Ellipse>&>(_context->getGLManager().getEllipses());
    vec.insert(vec.begin() + m_index, Ellipse(&_context->getGLManager(), m_posx, m_posy, m_sizex, m_sizey));
}