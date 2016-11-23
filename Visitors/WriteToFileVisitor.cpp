//
// Created by Hindrik Stegenga on 9-11-16.
//

#include "WriteToFileVisitor.h"
#include "../OpenGL/Drawer.h"
#include "../Shapes/Shape.h"

void WriteToFileVisitor::start_visit(Shape &_shape)
{
    glm::vec2 pos = call_automatic<glm::vec2>(&Shape::getPosition, _shape);
    glm::vec2 size = call_automatic<glm::vec2>(&Shape::getSize, _shape);

    string core = _shape.getDrawer() == nullptr ? throw "Invalid drawer!" : call_automatic<Drawer*>(&Shape::getDrawer,_shape)->toString();
    core.append(" ");
    core.append(to_string(static_cast<int>(pos.x - static_cast<int>(size.x / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(pos.y - static_cast<int>(size.y / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(size.x)));
    core.append(" ");
    core.append(to_string(static_cast<int>(size.y)));
    for(int i = 0; i < m_tabs; ++i)
        core.insert(core.begin(), 1, '\t');
    m_data.push_back(core);
}

void WriteToFileVisitor::start_visit(Group &_group)
{
    string core = "group ";
    core.append(to_string(static_cast<int>(call_automatic< vector<unique_ptr<Shape>>&, Group>(&Group::getShapes, _group).size())));
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
