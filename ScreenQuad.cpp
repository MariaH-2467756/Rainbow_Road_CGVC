#include "ScreenQuad.h"

#define VERTEX_SHADER_PATH "shaders/post_processing_vertex_shader.glsl"
#define FRAGMENT_SHADER_PATH "shaders/passtrough_fragment_shader.glsl"

std::unique_ptr<Shader> ScreenAllignedQuad::m_shader = nullptr;

ScreenAllignedQuad::ScreenAllignedQuad() {
  float quadVertices[] = {-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                          1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                          1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
}

void ScreenAllignedQuad::initShader() {
  m_shader = std::make_unique<Shader>(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

void ScreenAllignedQuad::draw() {
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void ScreenAllignedQuad::draw(GLuint texture) {
  m_shader->useProgram();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}