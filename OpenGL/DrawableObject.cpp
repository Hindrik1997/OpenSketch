//
// Created by Hindrik Stegenga on 26-9-16.
//

#include "DrawableObject.h"

DrawableObject::DrawableObject(vector<GLfloat> _vertices, vector<GLint> _indices) : m_vertices(_vertices), m_indices(_indices)
{
    resetObject();
}

DrawableObject::~DrawableObject() {

}

void DrawableObject::resetObject(vector<GLfloat> _vertices, vector<GLint> _indices) {
    m_vertices = _vertices;
    m_indices = _indices;
    resetObject();
}

void DrawableObject::resetObject()
{
    //Oude rommel wehalen indien nodig
    if(m_EBO != 0)
        glDeleteBuffers(1,&m_EBO);
    if(m_VBO != 0)
        glDeleteBuffers(1,&m_VBO);
    if(m_VAO != 0)
        glDeleteVertexArrays(1,&m_VAO);

    //Buffer id's fetchen
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);

    //alles moet in de VAO, dus die binden we hier.
    glBindVertexArray(m_VAO);

        //Vanaf nu wordt het aan de VAO gekoppeld

        //Vertex buffer koppelen
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        //Vertices copyen
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices[0], GL_STATIC_DRAW);

        //Index buffer koppelen
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        //indices copyen
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLint), &m_indices[0], GL_STATIC_DRAW);

        //vertex attributes instellen
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

        //Geen extra vertex data
        glEnableVertexAttribArray(0);

    //Unbinden van de VAO
    glBindVertexArray(0);


}

void DrawableObject::draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
