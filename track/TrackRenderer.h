#pragma once
#include "../includes/glad/glad.h"
#include "Track.h"

class TrackRenderer {
public:
    ~TrackRenderer() { cleanup(); }
    void upload(const Track& ribbon);
    void draw() const;

private:
    void cleanup();

    GLuint m_VAO = 0, m_VBO = 0;
    int m_vertexCount = 0;
};