//
// Created by hindrik on 5-10-16.
//

#include "RemoveRectangleCommand.h"
#include "../Shapes/Rectangle.h"
#include "../OpenGL/OpenGLRenderManager.h"

RemoveRectangleCommand::RemoveRectangleCommand(size_t _index, int _posx, int _posy, int _sizex, int _sizey)
    : m_index(_index), m_posx(_posx), m_posy(_posy), m_sizex(_sizex), m_sizey(_sizey)
{
}

void RemoveRectangleCommand::execute(Application *_context)
{
    vector<Rectangle>& vec = const_cast<vector<Rectangle>&>(_context->getGLManager().getRectangles());
    vec.erase(vec.begin() + m_index);
}

void RemoveRectangleCommand::undo(Application *_context)
{
    vector<Rectangle>& vec = const_cast<vector<Rectangle>&>(_context->getGLManager().getRectangles());
    vec.insert(vec.begin() + m_index, Rectangle(&_context->getGLManager(), m_posx, m_posy, m_sizex, m_sizey));
}