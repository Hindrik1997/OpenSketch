//
// Created by Hindrik Stegenga on 27-10-16.
//

#include <algorithm>
#include "FormGroupCommand.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../Visitors/SetSelectedVisitor.h"



//Debug code
#ifdef DEBUG

void dropVec()
{
    auto& vec = Application::getInstance().getGLManager().getShapes();

    for(int i = 0; i < 10; ++i)
        std::cout << "-";
    std::cout << std::endl;
    for(auto&& s : vec)
        std::cout << &*s << std::endl;
    for(int i = 0; i < 10; ++i)
        std::cout << "-";
    std::cout << std::endl;
}

void dropVec(vector<unique_ptr<Shape>>& vec)
{

    for(int i = 0; i < 10; ++i)
        std::cout << "-";
    std::cout << std::endl;
    for(auto&& s : vec)
        std::cout << &*s << std::endl;
    for(int i = 0; i < 10; ++i)
        std::cout << "-";
    std::cout << std::endl;
}

#endif

FormGroupCommand::FormGroupCommand(vector<size_t> _indices) : m_indices(_indices)
{

}

void FormGroupCommand::execute(Application *_context) {

    //sort it
    std::sort(m_indices.begin(), m_indices.end());
    //reverse it
    std::reverse(m_indices.begin(), m_indices.end());

    vector<unique_ptr<Shape>> temp;

    vector<unique_ptr<Shape>> &vec = _context->getGLManager().getShapes();

    SetSelectedVisitor v(false);

    //contructs temporary vector in which it moves the shapes we decided to group together
    for (auto &&index : m_indices)
    {
        vec[index]->accept(v);
        temp.emplace_back(std::move(vec[index]));
        vec.erase(vec.begin() + index);
    }

    _context->getSelectedShapes().clear();

    unique_ptr<Shape> group = std::make_unique<Group>(&_context->getGLManager(), temp);
    m_groupIndex = _context->getGLManager().addGroup(std::move(group)); //takes control of the unique ptr
}

void FormGroupCommand::undo(Application *_context)
{
    //does the exact opposite of the execute() function.
    //takes the sorted index list, instead of reversed
    //and also reverses the grouped set
    //which results into a reconstructed list as it was before

    vector<unique_ptr<Shape>>& vec = _context->getGLManager().getShapes();

    std::sort(m_indices.begin(), m_indices.end());

    unique_ptr<Shape> group = std::move(vec[m_groupIndex]); //Control of the resource (shape) moved to unique_ptr on stack
    vec.erase(vec.begin() + m_groupIndex);
    m_groupIndex = 0;

    Group* g = dynamic_cast<Group*>(group.get());
    if(g == nullptr) throw "FAILURE";
    vector<unique_ptr<Shape>>& gvec = g->getShapes();
    std::reverse(gvec.begin(), gvec.end());

    for(auto&& index : m_indices)
    {
        vec.insert(vec.begin() + index, std::move(gvec.front()));
        gvec.erase(gvec.begin());
    }

    _context->resetState();
}


