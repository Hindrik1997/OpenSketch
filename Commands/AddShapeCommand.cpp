//
// Created by hindrik on 5-10-16.
//

#include "AddShapeCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

void AddShapeCommand::execute(Application *_context)
{
    Shape& s = _context->getGLManager().addShape(m_posx, m_posy, m_sizex, m_sizey, m_drawer);
    m_index = _context->getGLManager().getIndex(&s);
}

void AddShapeCommand::undo(Application *_context)
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

AddShapeCommand::AddShapeCommand(int _posx, int _posy, int _sizex, int _sizey, Drawer* _drawer) : m_sizex(_sizex), m_sizey(_sizey), m_posx(_posx), m_posy(_posy), m_drawer(_drawer)
{
}