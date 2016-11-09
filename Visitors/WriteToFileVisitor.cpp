//
// Created by Hindrik Stegenga on 9-11-16.
//

#include "WriteToFileVisitor.h"
#include "../OpenGL/Drawer.h"
#include "../Shapes/Shape.h"

void WriteToFileVisitor::visit(Shape &_shape)
{
    std::string core = _shape.getDrawer() == nullptr ? throw "Invalid drawer!" : _shape.getDrawer()->toString();
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

    for(auto&& s : _group.getShapes())
    {
        WriteToFileVisitor v;
        s->accept(v);
        vector<string> t = v.getData();
        for(auto&& l : t)
        {
            l.insert(l.begin(),1,'\t');
        }

        m_data.reserve(m_data.size() + t.size());
        m_data.insert(m_data.end(), t.begin(), t.end());
    }
}

vector<string> WriteToFileVisitor::getData() {
    return m_data;
}
