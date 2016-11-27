//
// Created by Hindrik Stegenga on 26-9-16.
//

#include <cmath>
#include "ShapeRenderManager.h"
#include "../Visitors/DrawVisitor.h"
#include "../Decorators/OrnamentTopDecorator.h"
#include "../Decorators/OrnamentBottomDecorator.h"
#include "../Decorators/OrnamentLeftDecorator.h"
#include "../Decorators/OrnamentRightDecorator.h"

void ShapeRenderManager::render()
{

        //Clear color state setten
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        //Clear color state fetchen en enkel de color buffer clearen. (Niet de depth/stencil enz,
        // deze gebruik ik niet in deze applicatie)
        glClear(GL_COLOR_BUFFER_BIT);

        DrawVisitor v;
        for(auto&& s : m_shapes)
        {
            s->accept(v);
        }

        setNullShaderProgram();

    //Buffer swappen om te laten zien
    glfwSwapBuffers(m_application.getM_paintWindow());

}

ShapeRenderManager::ShapeRenderManager(Application& _app) : m_application(_app)
{
}

void ShapeRenderManager::setCustomShaderProgram(const GLuint& _shaderProgam)
{
    glUseProgram(_shaderProgam);
}

void ShapeRenderManager::setNullShaderProgram()
{
    GLuint t = 0;
    setCustomShaderProgram(t);
}

void ShapeRenderManager::createCustomShaderProgam(string _vertexShader, string _fragmentShader, GLuint& _shaderID)
{
    const int debugBufSize = 524; //524 is groot genoeg voor error messages. gwn random groot genoeg nummer.
    GLint success;
    GLchar debugMsg[debugBufSize];


    //files inlezen
    ifstream vertex(_vertexShader);
    ifstream fragment(_fragmentShader);

    string vSource((istreambuf_iterator<char>(vertex)), istreambuf_iterator<char>());
    string fSource((istreambuf_iterator<char>(fragment)), istreambuf_iterator<char>());

    const char* vp = vSource.c_str();
    const char* fp = fSource.c_str();

    //Shaders compilen

    GLuint vShader, fShader;
    vShader= glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vp, NULL);
    glShaderSource(fShader, 1, &fp, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);

    //Check of alles goed gelukt is

    //vertex shader
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vShader, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED COMPILATION VERTEXSHADER";
    }
    //fragment shader
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fShader, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED COMPILATION FRAGMENTSHADER";
    }

    //compilatie gelukt!
    //Shader program linken!
    _shaderID = glCreateProgram();
    glAttachShader(_shaderID, vShader);
    glAttachShader(_shaderID, fShader);
    glLinkProgram(_shaderID);

    //Checken of linken gelukt is!
    glGetProgramiv(_shaderID, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(_shaderID, debugBufSize, NULL, debugMsg);
        cout << debugMsg << endl;
        throw "FAILED LINKING SHADERPROGRAM";
    }

    //Rotzooi opruimen
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    //klaar!
}

Shape& ShapeRenderManager::addShape(int _posx, int _posy, int _width, int _height, Drawer* _drawer)
{
    m_shapes.push_back(std::unique_ptr<Shape>(new Shape(_posx, _posy, _width, _height, _drawer)));
    return *m_shapes.back();
}

Shape* ShapeRenderManager::getSelectedShape() {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    Shape* found = nullptr;

    for(size_t i = 0; i < m_shapes.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_shapes[i]->getPosition().x - (m_shapes[i]->getSize().x / 2));
        xright = static_cast<int>(m_shapes[i]->getPosition().x + (m_shapes[i]->getSize().x / 2));
        ytop = static_cast<int>(m_shapes[i]->getPosition().y - (m_shapes[i]->getSize().y / 2));
        ybottom = static_cast<int>(m_shapes[i]->getPosition().y + (m_shapes[i]->getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
            found = &(*m_shapes[i]);
    }
    return found;
}

glm::vec2 ShapeRenderManager::getMouseOffsetInShape(Shape& _rect,int _mousex, int _mousey) {
    int middleX, middleY;
    middleX = static_cast<int>(_rect.getPosition().x);
    middleY = static_cast<int>(_rect.getPosition().y);

    glm::vec2 result;

    if (middleX > _mousex)
        result.x = abs(middleX - _mousex);
    else
        result.x = -abs(middleX - _mousex);

    if(middleY > _mousey)
        result.y = abs(middleY - _mousey);
    else
        result.y = - abs(middleY - _mousey);
    return result;
}

Shape *ShapeRenderManager::getSelectedShapePriority(Shape* _shape) {
    int posx, posy;
    glm::vec2 pos = m_application.getPaintWindowCursorPos();
    posx = static_cast<int>(pos.x); posy = static_cast<int>(pos.y);

    int indexSecond = -1;

    for(size_t i = 0; i < m_shapes.size(); ++i)
    {
        int xleft, xright, ytop, ybottom;
        xleft = static_cast<int>(m_shapes[i]->getPosition().x - (m_shapes[i]->getSize().x / 2));
        xright = static_cast<int>(m_shapes[i]->getPosition().x + (m_shapes[i]->getSize().x / 2));
        ytop = static_cast<int>(m_shapes[i]->getPosition().y - (m_shapes[i]->getSize().y / 2));
        ybottom = static_cast<int>(m_shapes[i]->getPosition().y + (m_shapes[i]->getSize().y / 2));

        if(posx >= xleft && posx <= xright && posy <= ybottom && posy >= ytop)
        {
            if(&(*m_shapes[i]) == _shape)
                return &(*m_shapes[i]);
            else
                indexSecond = static_cast<int>(i);
        }
    }
    if(indexSecond != -1)
        return &(*m_shapes[indexSecond]);
    return nullptr;
}

size_t ShapeRenderManager::getIndex(Shape* _shape)
{
    for(size_t i = 0; i != m_shapes.size(); ++i)
    {
        if(m_shapes[i].get() == _shape)
            return static_cast<size_t>(i);
    }
    throw "Out of range";
}

size_t ShapeRenderManager::addGroup(unique_ptr<Shape> _ptr) {
    m_shapes.emplace_back(std::move(_ptr));
    return m_shapes.size() - 1;
}