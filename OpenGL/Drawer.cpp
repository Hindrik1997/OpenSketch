//
// Created by Hindrik Stegenga on 20-10-16.
//

#include "Drawer.h"

Drawer::~Drawer()
{
    glUseProgram(0);
    glDeleteShader(m_shader);
}

void Drawer::draw(const Shape& _shape) const
{
    ShapeRenderManager::setCustomShaderProgram(m_shader);

    GLint transformLocation = glGetUniformLocation(m_shader, "transformMatrix");
    GLint isSelLocation = glGetUniformLocation(m_shader, "isSelected");

    GLint widthLocation = glGetUniformLocation(m_shader, "width");
    GLint heightLocation = glGetUniformLocation(m_shader, "height");

    glUniform1i(isSelLocation, _shape.getSelected());
    glUniform1i(widthLocation, static_cast<GLint>(_shape.getSize().x));
    glUniform1i(heightLocation, static_cast<GLint>(_shape.getSize().y));

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(_shape.getMatrix()));
    DrawableObject::getInstance().draw();
    ShapeRenderManager::setNullShaderProgram();
}

Drawer::Drawer(std::string _vshader, std::string _fshader) {
    ShapeRenderManager::createCustomShaderProgam(_vshader, _fshader, m_shader);
}