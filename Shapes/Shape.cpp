//
// Created by hindrik on 4-10-16.
//

#include "Shape.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../OpenGL/Drawer.h"
#include "Visitor.h"

//mappen van range x-y naar w-z
static float map_range(float _value, float _from1, float _to1, float _from2, float _to2)
{
    return _from2 + (_value - _from1) * (_to2 - _from2) / (_to1 - _from1);
}

Shape::Shape(int _posx, int _posy, int _width, int _height, Drawer* _drawer)
        :  m_drawer(_drawer)
{
    //Basic Shape in t midden, deze transformeer ik doormiddel van matrices naar de juiste positie en grootte

    resize(_width, _height);
    move(_posx, _posy);
}

Shape::~Shape() {
}

void Shape::draw() const {
    if(m_drawer != nullptr)
        m_drawer->draw(*this);
}

//Visit() implementatie voor de shape klasse
void Shape::accept(Visitor &_v) {
    _v.start_visit(*this);
    _v.stop_visit(*this);
}

//Resized een shape met _pixelsX en _pixelsY in respectievelijk x en y assen
void Shape::resize(int _pixelsX, int _pixelsY) {
    int width = static_cast<int>(getSize().x) + _pixelsX, height = static_cast<int>(getSize().y) + _pixelsY;

    int window_width, window_height;
    Application::getInstance().getPaintWindowSize(window_width, window_height);

    int defaultSizeX = static_cast<int>(0.5f * window_width);
    int defaultSizeY = static_cast<int>(0.5f * window_height);

    //Mappen van de pixel coord naar Normalized Device Coords range. (0 tot width/height naar -1 tot 1)
    float scalex = map_range(width, 0, defaultSizeX, 0.0f, 1.0f);
    float scaley = map_range(height, 0, defaultSizeY, 0.0f, 1.0f);

    //Deze transform op de matrix toepassen, we moeten wel de translatie behouden, immers die bepaalt de positie.
    glm::vec3 translate;
    glm::quat dummy0;
    glm::vec3 dummy1;
    glm::vec4 dummy2;
    glm::decompose(m_transformMatrix, dummy1, dummy0,translate,dummy1,dummy2);
    glm::mat4 identity;
    identity = glm::translate(identity, translate);
    m_transformMatrix = glm::scale(identity, glm::vec3(scalex, scaley, 1.0f));
    m_size = glm::vec2(width, height);
}

//Moved een shape met _pixelsX en _pixelsY in respectievelijk x en y assen
void Shape::move(int _pixelsX, int _pixelsY) {

    int width = static_cast<int>(getPosition().x) + _pixelsX, height = static_cast<int>(getPosition().y) + _pixelsY;

    int window_width, window_height;
    Application::getInstance().getPaintWindowSize(window_width, window_height);

    //Mappen van de pixel coord naar Normalized Device Coords range. (0 tot width/height naar -1 tot 1)
    float posx = map_range(width, 0, window_width, -1.0f, 1.0f);
    float posy = map_range(height, 0, window_height, 1.0f, -1.0f);

    //Deze transform op de matrix toepassen, we moeten wel de scale behouden, immers die bepaalt de grootte.
    glm::vec3 scale;
    glm::quat dummy0;
    glm::vec3 dummy1;
    glm::vec4 dummy2;
    glm::decompose(m_transformMatrix, scale, dummy0,dummy1,dummy1,dummy2);
    glm::mat4 identity;
    m_transformMatrix = glm::translate(identity, glm::vec3(posx, posy, 0.0f));
    m_transformMatrix = glm::scale(m_transformMatrix, scale);
    m_position = glm::vec2(width, height);
}

vector<string> Shape::getDrawInformation() const {
    vector<string> data;
    if(m_drawer != nullptr)
    {
        data.push_back(m_drawer->toString());
    }
    return data;
}