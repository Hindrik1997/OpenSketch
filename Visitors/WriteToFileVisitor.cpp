//
// Created by Hindrik Stegenga on 9-11-16.
//

#include "WriteToFileVisitor.h"
#include "../OpenGL/Drawer.h"

void WriteToFileVisitor::start_visit(Shape &_shape)
{
    glm::vec2 pos = call_automatic<glm::vec2>(&Shape::getPosition, _shape);
    glm::vec2 size = call_automatic<glm::vec2>(&Shape::getSize, _shape);

    vector<string> data;
    vector<string> temp = call_automatic<vector<string>>(&Shape::getDrawInformation, _shape);
    for(auto it = temp.begin(); it != temp.end() - 1; ++it)
    {
        data.push_back(*it);
    }
    string core;
    core.append(temp.back());
    core.append(" ");
    core.append(to_string(static_cast<int>(pos.x - static_cast<int>(size.x / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(pos.y - static_cast<int>(size.y / 2))));
    core.append(" ");
    core.append(to_string(static_cast<int>(size.x)));
    core.append(" ");
    core.append(to_string(static_cast<int>(size.y)));
    data.push_back(core);

    for(auto&& s : data)
    {
        for(int i = 0; i < m_tabs; ++i)
        {
            s.insert(s.begin(), 1, '\t');
        }
    }
    m_data.insert(std::end(m_data), std::begin(data), std::end(data));
}

void WriteToFileVisitor::start_visit(Group &_group)
{
    vector<string> data;

    vector<string> temp = call_automatic<vector<string>>(&Shape::getDrawInformation, _group);
    data.insert(std::begin(data), std::begin(temp), std::end(temp));
    for(auto&& s : data)
    {
        for(int i = 0; i < m_tabs; ++i)
            s.insert(s.begin(), 1, '\t');
    }
    m_data.insert(std::end(m_data), std::begin(data), std::end(data));
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
