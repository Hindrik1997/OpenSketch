//
// Created by hindrik on 5-10-16.
//

#include <algorithm>
#include "RemoveShapeCommand.h"
#include "../OpenGL/ShapeRenderManager.h"

RemoveShapeCommand::RemoveShapeCommand(size_t _index)
        : m_index(_index)
{
}

void RemoveShapeCommand::execute(Application *_context)
{
    vector<unique_ptr<Shape>>& vec = const_cast<vector<unique_ptr<Shape>>&>(_context->getGLManager().getShapes());

    m_data = std::move(vec[m_index]);
    vec.erase(vec.begin() + m_index);
}

void RemoveShapeCommand::undo(Application *_context)
{
    vector<unique_ptr<Shape>>& vec = const_cast<vector<unique_ptr<Shape>>&>(_context->getGLManager().getShapes());
    vec.insert(vec.begin() + m_index, unique_ptr<Shape>(std::move(m_data)));
    m_data.reset(nullptr);
}