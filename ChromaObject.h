#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ChromaObject {
public:
  ChromaObject(const char *textureFilePath);
  ~ChromaObject();

  static void initShader();
  void draw(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &projection);

private:
  void setMVP(const glm::mat4 &model, const glm::mat4 &view,
              const glm::mat4 &projection);
  void init();
  void cleanup();

  static std::unique_ptr<Shader> m_shader;
  GLuint m_texture;
  GLuint m_VAO;
  GLuint m_VBO;
};