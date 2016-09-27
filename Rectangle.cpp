//
// Created by Hindrik Stegenga on 27-9-16.
//

#include "Rectangle.h"
#include "OpenGL/OpenGLRenderManager.h"
#include "Application.h"

float map(float _value, float _from1, float _to1, float _from2, float _to2)
{
    return _from2 + (_value - _from1) * (_to2 - _from2) / (_to1 - _from1);
}


Rectangle::Rectangle(OpenGLRenderManager &_oglRenderer,int _posx, int _posy, int _width, int _height) : m_oglRenderer(_oglRenderer)
{
    //Basic rectangle in t midden, deze transformeer ik doormiddel van matrices naar de juiste positie en grootte
    m_oglRenderer.addRenderObject(vector<GLfloat>{
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
    }, vector<GLint>{
            0, 1, 3,
            1, 2, 3
    });
    m_drawableObjectindex = static_cast<int>(m_oglRenderer.getDrawables().size() - 1);
    setPosition(_posx, _posy);
    setSize(_width, _height);
}

void Rectangle::setPosition(int _pixelsX, int _pixelsY)
{
    int window_width, window_height;
    Application::getInstance().getPaintWindowSize(window_width, window_height);

    //Mappen van de pixel coord naar Normalized Device Coords range. (0 tot width/height naar -1 tot 1)
    float posx = map(_pixelsX, 0, window_width, -1.0f, 1.0f);
    float posy = map(_pixelsY, 0, window_height, 1.0f, -1.0f);

    //Deze transform op de matrix toepassen, we moeten wel de scale behouden, immers die bepaalt de grootte.
    glm::vec3 scale;
    glm::quat dummy0;
    glm::vec3 dummy1;
    glm::vec4 dummy2;
    glm::decompose(m_transformMatrix, scale, dummy0,dummy1,dummy1,dummy2);
    glm::mat4 identity;
    m_transformMatrix = glm::translate(identity, glm::vec3(posx, posy, 0.0f));
    m_transformMatrix = glm::scale(m_transformMatrix, scale);
}

void Rectangle::setSize(int _width, int _height)
{
    int window_width, window_height;
    Application::getInstance().getPaintWindowSize(window_width, window_height);


    int defaultSizeX = static_cast<int>(0.5f * window_width);
    int defaultSizeY = static_cast<int>(0.5f * window_height);

    //Mappen van de pixel coord naar Normalized Device Coords range. (0 tot width/height naar -1 tot 1)
    float scalex = map(_width, 0, defaultSizeX, 0.0f, 1.0f);
    float scaley = map(_height, 0, defaultSizeY, 0.0f, 1.0f);

    //Deze transform op de matrix toepassen, we moeten wel de translatie behouden, immers die bepaalt de positie.
    glm::vec3 translate;
    glm::quat dummy0;
    glm::vec3 dummy1;
    glm::vec4 dummy2;
    glm::decompose(m_transformMatrix, dummy1, dummy0,translate,dummy1,dummy2);
    glm::mat4 identity;
    m_transformMatrix = glm::scale(identity, glm::vec3(scalex, scaley, 1.0f));
    m_transformMatrix = glm::translate(m_transformMatrix, translate);
}

const DrawableObject& Rectangle::getDrawObject() const
{
    return m_oglRenderer.getDrawables()[m_drawableObjectindex];
}