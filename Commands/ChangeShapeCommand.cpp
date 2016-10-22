//
// Created by hindrik on 5-10-16.
//

#include <algorithm>
#include "ChangeShapeCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

ChangeShapeCommand::ChangeShapeCommand(vector<int> _index,int _posx_change, int _posy_change, int _sizex_change, int _sizey_change)
        : m_posx_change(_posx_change), m_posy_change(_posy_change), m_sizex_change(_sizex_change), m_sizey_change(_sizey_change), m_index(_index)
{
    std::reverse(m_index.begin(), m_index.end());
}

void ChangeShapeCommand::execute(Application *_context)
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


    Shape& rect = const_cast<Shape&>(g->operator[](lastIndex));
    glm::vec2 pos = rect.getPosition();
    glm::vec2 size = rect.getSize();
    rect.setPosition(static_cast<int>(pos.x) + m_posx_change, static_cast<int>(pos.y) + m_posy_change);
    rect.setSize(static_cast<int>(size.x) + m_sizex_change, static_cast<int>(size.y) + m_sizey_change);
}

void ChangeShapeCommand::undo(Application *_context)
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


    Shape& rect = const_cast<Shape&>(g->operator[](lastIndex));
    glm::vec2 pos = rect.getPosition();
    glm::vec2 size = rect.getSize();
    rect.setPosition(static_cast<int>(pos.x) - m_posx_change, static_cast<int>(pos.y) - m_posy_change);
    rect.setSize(static_cast<int>(size.x) - m_sizex_change, static_cast<int>(size.y) - m_sizey_change);
}