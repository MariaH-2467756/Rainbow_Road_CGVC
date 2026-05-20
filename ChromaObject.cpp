#include "ChromaObject.h"
#include "ObjLoader.h"
#include <glm/detail/qualifier.hpp>

#define VERTEX_SHADER_PATH "shaders/chroma_vertex_shader.glsl"
#define FRAGMENT_SHADER_PATH "shaders/chroma_fragment_shader.glsl"

std::unique_ptr<Shader> ChromaObject::m_shader = nullptr;

ChromaObject::ChromaObject(const char *textureFilePath) : m_VAO(0), m_VBO(0) {
  loadTexture(textureFilePath, m_texture);
  init();
}

ChromaObject::~ChromaObject() { cleanup(); }

void ChromaObject::initShader() {
  m_shader = std::make_unique<Shader>(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

void ChromaObject::setMVP(const glm::mat4 &model, const glm::mat4 &view,
                          const glm::mat4 &projection) {
  m_shader->setMat4("model", model);
  m_shader->setMat4("view", view);
  m_shader->setMat4("projection", projection);
}

void ChromaObject::init() {
  float quadVertices[] = {// pos (x,y)   // texCoord
                          -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  glDisable(GL_DEPTH_TEST);

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
}

void ChromaObject::draw(const glm::mat4 &model, const glm::mat4 &view,
                        const glm::mat4 &projection) {

  m_shader->useProgram();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  m_shader->setIntUniform("chromaTexture", 0);
  setMVP(model, view, projection);

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void ChromaObject::cleanup() {
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
}