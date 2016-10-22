//
// Created by hindrik on 5-10-16.
//

#include <algorithm>
#include "RemoveShapeCommand.h"
#include "../Shapes/Shape.h"
#include "../OpenGL/OpenGLRenderManager.h"

RemoveShapeCommand::RemoveShapeCommand(std::vector<int> _index, int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer)
        : m_index(_index), m_posx(_posx), m_posy(_posy), m_sizex(_sizex), m_sizey(_sizey), m_drawer(_drawer)
{
    std::reverse(m_index.begin(), m_index.end());
}

void RemoveShapeCommand::execute(Application *_context)
{
    vector<int> temp = m_index;
    Group* g = const_cast<Group*>(&_context->getGLManager().getRootGroup());
    while(temp.size() != 1)
    {
        int index = temp.back();
        temp.pop_back();
        g = dynamic_cast<Group*>(&g->operator[](index));
    }

    int lastIndex = temp.back();
    g->getShapeVector().erase(g->getShapeVector().begin() + lastIndex);
}

void RemoveShapeCommand::undo(Application *_context)
{
    vector<int> temp = m_index;
    Group* g = const_cast<Group*>(&_context->getGLManager().getRootGroup());
    while(temp.size() != 1)
    {
        int index = temp.back();
        temp.pop_back();
        g = dynamic_cast<Group*>(&g->operator[](index));
    }

    int lastIndex = temp.back();
    g->getShapeVector().insert(g->getShapeVector().begin() + lastIndex, Shape(&_context->getGLManager(), m_posx, m_posy, m_sizex, m_sizey, m_drawer));
}