#include "Mesh.h"
#include <utility>


Mesh::Mesh(Mesh&& other) noexcept: m_VAO(std::exchange(other.m_VAO, 0)), m_VBO(std::exchange(other.m_VBO, 0)), m_EBO(std::exchange(other.m_EBO, 0)), 
m_vertexCount(std::exchange(other.m_vertexCount, 0)), m_indexCount (std::exchange(other.m_indexCount,  0)) {}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();
        m_VAO         = std::exchange(other.m_VAO, 0);
        m_VBO         = std::exchange(other.m_VBO, 0);
        m_EBO         = std::exchange(other.m_EBO, 0);
        m_vertexCount = std::exchange(other.m_vertexCount, 0);
        m_indexCount  = std::exchange(other.m_indexCount,  0);
    }
    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0); // reset bind.
}

void Mesh::setupBuffers(const MeshData& data) {
    m_vertexCount = data.vertices.size();
    m_indexCount  = data.indices.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertexCount * sizeof(Vertex)), data.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indexCount * sizeof(unsigned int)), data.indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

    glBindVertexArray(0); // reset bind.
}

void Mesh::release() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
    m_VAO = m_VBO = m_EBO = 0;
}