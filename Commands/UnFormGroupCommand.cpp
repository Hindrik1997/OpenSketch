//
// Created by Hindrik Stegenga on 31-10-16.
//

#include "UnFormGroupCommand.h"
#include "../Application.h"
#include "../Shapes/Group.h"
#include "../OpenGL/OpenGLRenderManager.h"

UnFormGroupCommand::UnFormGroupCommand(size_t _index) : m_groupIndex(_index)
{

}

void UnFormGroupCommand::execute(Application *_context)
{
    vector<unique_ptr<Shape>>& vec = const_cast<vector<unique_ptr<Shape>>&>(_context->getGLManager().getShapes());
    m_shapesIndices.clear();
    m_group = std::move(vec[m_groupIndex]); //Control of the resource (shape) moved to unique_ptr
    vec.erase(vec.begin() + m_groupIndex);
    m_groupIndex = 0;

    Group* g = dynamic_cast<Group*>(m_group.get());
    if(g == nullptr) throw "FAILURE";
    vector<unique_ptr<Shape>>& gvec = g->getShapes();

    for(auto&& shape : gvec)
    {
        shape->setSelected(false);
        vec.emplace_back(std::move(gvec.back()));
        gvec.erase(gvec.end() - 1);
        m_shapesIndices.push_back(vec.size() - 1);
    }

    _context->resetState();
}

void UnFormGroupCommand::undo(Application *_context)
{
    vector<unique_ptr<Shape>> &vec = const_cast<vector<unique_ptr<Shape>> &>(_context->getGLManager().getShapes());

    Group* g = dynamic_cast<Group*>(m_group.get());
    if(g == nullptr) throw "FAILURE";

    for (auto &&index : m_shapesIndices)
    {
        g->getShapes().emplace_back(std::move(vec[index]));
        vec.erase(vec.begin() + index);
    }

    _context->getSelectedShapes().clear();

    vec.insert(vec.begin() + m_groupIndex, std::move(m_group));
    m_group.reset(nullptr);
}
