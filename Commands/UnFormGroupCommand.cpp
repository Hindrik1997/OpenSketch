//
// Created by Hindrik Stegenga on 31-10-16.
//

#include "UnFormGroupCommand.h"
#include "../Application.h"
#include "../Shapes/Group.h"
#include "../OpenGL/ShapeRenderManager.h"

UnFormGroupCommand::UnFormGroupCommand(size_t _index) : m_groupIndex(_index)
{

}

void UnFormGroupCommand::execute(Application *_context)
{
    vector<unique_ptr<Shape>>& vec = _context->getGLManager().getShapes();
    m_group = std::move(vec[m_groupIndex]); //Control of the resource (shape) moved to unique_ptr
    vec.erase(vec.begin() + m_groupIndex);
    m_groupIndex = 0;

    Group* g = dynamic_cast<Group*>(m_group.get());
    if(g == nullptr) throw "FAILURE";
    vector<unique_ptr<Shape>>& gvec = g->getShapes();


    m_groupSize = gvec.size();
    for(size_t i = 0; i < m_groupSize; ++i)
    {
        gvec[gvec.size() - 1]->setSelected(false);
        vec.emplace_back(std::move(gvec[gvec.size() - 1]));
        gvec.erase(gvec.end() - 1);
    }
    _context->resetState();
}

void UnFormGroupCommand::undo(Application *_context)
{
    vector<unique_ptr<Shape>> &vec = _context->getGLManager().getShapes();

    Group* g = dynamic_cast<Group*>(m_group.get());
    if(g == nullptr) throw "FAILURE";

    for (size_t i = 0; i < m_groupSize; ++i)
    {
        g->getShapes().emplace_back(std::move(vec[vec.size() - 1]));
        vec.erase(vec.end() - 1);
    }
    std::reverse(g->getShapes().begin(), g->getShapes().end());
    _context->getSelectedShapes().clear();

    vec.insert(vec.begin() + m_groupIndex, std::move(m_group));
    m_group.reset(nullptr);
}
