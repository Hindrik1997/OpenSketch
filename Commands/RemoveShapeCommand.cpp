//
// Created by hindrik on 5-10-16.
//

#include "RemoveShapeCommand.h"
#include "../Shapes/Shape.h"
#include "../OpenGL/OpenGLRenderManager.h"

RemoveShapeCommand::RemoveShapeCommand(size_t _index, int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer)
        : m_index(_index), m_posx(_posx), m_posy(_posy), m_sizex(_sizex), m_sizey(_sizey), m_drawer(_drawer)
{
}

void RemoveShapeCommand::execute(Application *_context)
{
    vector<Shape>& vec = const_cast<vector<Shape>&>(_context->getGLManager().getShapes());
    vec.erase(vec.begin() + m_index);
}

void RemoveShapeCommand::undo(Application *_context)
{
    vector<Shape>& vec = const_cast<vector<Shape>&>(_context->getGLManager().getShapes());
    vec.insert(vec.begin() + m_index, Shape(&_context->getGLManager(), m_posx, m_posy, m_sizex, m_sizey, m_drawer));
}