//
// Created by Hindrik Stegenga on 27-10-16.
//

#include <algorithm>
#include "FormGroupCommand.h"
#include "../OpenGL/OpenGLRenderManager.h"

FormGroupCommand::FormGroupCommand(vector<size_t> _indices) : m_indices(_indices)
{

}

void FormGroupCommand::execute(Application *_context) {

    //sort it
    std::sort(m_indices.begin(), m_indices.end());
    //reverse it
    std::reverse(m_indices.begin(), m_indices.end());


    vector<unique_ptr<Shape>> &vec = const_cast<vector<unique_ptr<Shape>> &>(_context->getGLManager().getShapes());

    for (auto &&index : m_indices)
    {
        vec[index]->setSelected(false);
        m_temp.emplace_back(std::move(vec[index]));
        vec.erase(vec.begin() + index);
    }

    _context->getSelectedShapes().clear();


    /*
    unique_ptr<Shape> group = std::make_unique<Group>(&_context->getGLManager(), tvec);
    m_groupIndex = _context->getGLManager().addGroup(std::move(group)); */
}

void FormGroupCommand::undo(Application *_context)
{
    vector<unique_ptr<Shape>>& vec = const_cast<vector<unique_ptr<Shape>>&>(_context->getGLManager().getShapes());

    std::reverse(m_indices.begin(), m_indices.end());


    for(int i = 0; i < m_temp.size(); ++i)
    {
        vec.insert(vec.begin() + m_indices[i], std::move(m_temp[i]));
    }
    m_temp.clear();

    std::reverse(m_indices.begin(), m_indices.end());

    _context->resetState();

    /*
    vector<unique_ptr<Shape>>& vec = const_cast<vector<unique_ptr<Shape>>&>(_context->getGLManager().getShapes());

    unique_ptr<Shape> group = std::move(vec[m_groupIndex]);
    vec.erase(vec.begin() + m_groupIndex);
    m_groupIndex = 0;

    //nu de shapes uit de group moven en in de originele posities plaatsen.
    Group* g = dynamic_cast<Group*>(group.get());
    if(g == nullptr) throw "FAILURE";
    vector<unique_ptr<Shape>>& gvec = g->getShapes();

    for(int i = 0; i < gvec.size(); ++i)
    {
        vec.insert(vec.begin() + m_indices[i], std::move(gvec[i]));
        gvec.erase(gvec.begin() + i);
    }
    */
    //group gets killed here
}


