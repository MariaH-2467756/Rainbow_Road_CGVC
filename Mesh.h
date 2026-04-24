#pragma once

#include <vector>
#include <string>
#include "includes/glad/glad.h"
#include <glm/glm.hpp>
class Shader;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct MeshData {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int texture;
};

class Mesh {
public:
    Mesh(const MeshData& data) { setupBuffers(data); };
    ~Mesh() { release(); };
    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    void draw() const;
    unsigned int vertexCount() const { return static_cast<unsigned int>(m_vertexCount); }
    unsigned int indexCount()  const { return static_cast<unsigned int>(m_indexCount);  }

private:
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;

    size_t m_vertexCount = 0;
    size_t m_indexCount  = 0;
    unsigned int m_texture;

    void setupBuffers(const MeshData& data);
    void release();
};