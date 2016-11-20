//
// Created by Hindrik Stegenga on 21-10-16.
//

#include "Group.h"
#include "Visitor.h"
#include "../Visitors/MoveShapeVisitor.h"
#include <algorithm>

Group::Group(vector<unique_ptr<Shape>>& _vec) : Shape(0,0,0,0, nullptr)
{
    //move de group members in de group vector.
    for(auto&& s : _vec)
    {
        m_shapes.emplace_back(std::move(s));
    }
    _vec.clear();
}

//Retourneert de positie van de group aan de hand van calculateMetrics()
glm::vec2 Group::getPosition() const {
    glm::vec2 result;
    int xmin,xmax,ymin,ymax;
    calculateMetrics(xmin,xmax,ymin,ymax);

    result.y = (ymin + ymax) / 2;
    result.x = (xmin + xmax) / 2;
    return result;
}

//Retourneert de grootte van de group aan de hand van calculateMetrics()
glm::vec2 Group::getSize() const {
    glm::vec2 result;
    int xmin,xmax,ymin,ymax;
    calculateMetrics(xmin,xmax,ymin,ymax);
    result.x = xmax - xmin;
    result.y = ymax - ymin;
    return result;
}

vector<unique_ptr<Shape>>& Group::getShapes() {
    return m_shapes;
}

//Berekent de boundaries van de group en geeft deze als ref params terug.
void Group::calculateMetrics(int& _minx, int& _maxx, int& _miny, int& _maxy) const {

    int min_x = std::numeric_limits<int>::max(),
            max_x = std::numeric_limits<int>::min(),
            min_y = std::numeric_limits<int>::max(),
            max_y = std::numeric_limits<int>::min();

    for(auto&& s : m_shapes)
    {
        int txmin = static_cast<int>(static_cast<int>(s->getPosition().x) - (static_cast<int>(s->getSize().x) / 2)),
                tymin = static_cast<int>(static_cast<int>(s->getPosition().y) - (static_cast<int>(s->getSize().y) / 2)),
                txmax = static_cast<int>(static_cast<int>(s->getPosition().x) + (static_cast<int>(s->getSize().x) / 2)),
                tymax = static_cast<int>(static_cast<int>(s->getPosition().y) + (static_cast<int>(s->getSize().y) / 2));

        if(txmin < min_x) min_x = txmin;
        if(txmax > max_x) max_x = txmax;
        if(tymin < min_y) min_y = tymin;
        if(tymax > max_y) max_y = tymax;
    }
    _minx = min_x;
    _maxx = max_x;
    _miny = min_y;
    _maxy = max_y;
}

//Accept voor de visitor van de group.
void Group::accept(Visitor &_v) {
    _v.start_visit(*this);
    for(auto&& s : m_shapes)
    {
        s->accept(_v);
    }
    _v.stop_visit(*this);
}