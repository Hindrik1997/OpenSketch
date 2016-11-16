//
// Created by Hindrik Stegenga on 9-11-16.
//

#include "WriteToFileVisitor.h"
#include "../OpenGL/Drawer.h"
#include "../Shapes/Shape.h"

void WriteToFileVisitor::start_visit(Shape &_shape)
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
    for(int i = 0; i < m_tabs; ++i)
        core.insert(core.begin(), 1, '\t');
    m_data.push_back(core);
}

void WriteToFileVisitor::start_visit(Group &_group)
{
    string core = "group ";
    core.append(to_string(static_cast<int>(_group.getShapes().size())));
    for(int i = 0; i < m_tabs; ++i)
        core.insert(core.begin(), 1, '\t');
    m_data.push_back(core);
    ++m_tabs;
}

vector<string> WriteToFileVisitor::getData() {
    return m_data;
}

void WriteToFileVisitor::stop_visit(Shape &_shape) {

}

void WriteToFileVisitor::stop_visit(Group &_group) {
    --m_tabs;
}
