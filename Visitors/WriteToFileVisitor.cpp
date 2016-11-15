//
// Created by Hindrik Stegenga on 9-11-16.
//

#include "WriteToFileVisitor.h"
#include "../OpenGL/Drawer.h"
#include "../Shapes/Shape.h"

void WriteToFileVisitor::visit(Shape &_shape)
{
    string core = _shape.getDrawer() == nullptr ? throw "Invalid drawer!" : _shape.getDrawer()->toString();
    core.append(" ");
    core.append(to_string(static_cast<int>(_shape.getPosition().x - static_cast<int>(_shape.getSize().x / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(_shape.getPosition().y - static_cast<int>(_shape.getSize().y / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(_shape.getSize().x)));
    core.append(" ");
    core.append(to_string(static_cast<int>(_shape.getSize().y)));
    m_data.push_back(core);
}

void WriteToFileVisitor::visit(Group &_group)
{
    string core = "group ";
    core.append(to_string(static_cast<int>(_group.getShapes().size())));
    m_data.push_back(core);

    /*
    for(auto&& s : _group.getShapes())
    {
        size_t length = m_data.size();

        s->accept(*this);

        for(size_t i = m_data.size() - 1; i >= length; --i )
        {
            m_data[i].insert(m_data[i].begin(), 1, '\t');
        }
    }*/
}

vector<string> WriteToFileVisitor::getData() {
    return m_data;
}
