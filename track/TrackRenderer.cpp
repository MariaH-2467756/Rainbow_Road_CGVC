#include "TrackRenderer.h"

void TrackRenderer::upload(const Track& ribbon) 
{
    m_vertexCount = ribbon.vertexCount();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER,
                    m_vertexCount * sizeof(TrackVertex),
                    ribbon.vertices().data(),
                    GL_STATIC_DRAW);

    // position → location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TrackVertex),
        (void*)offsetof(TrackVertex, position));
    glEnableVertexAttribArray(0);

    // normal → location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TrackVertex),
        (void*)offsetof(TrackVertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void TrackRenderer::draw() const 
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
    glBindVertexArray(0);
}

void TrackRenderer::cleanup() 
{
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
}
