#pragma once
#include "Shader.h"
#include "includes/glad/glad.h"
#include <memory>

class ScreenAllignedQuad {
public:
  ScreenAllignedQuad();
  static void initShader();
  void draw();
  void draw(GLuint texture);

private:
  unsigned int m_vao, m_vbo;
  static std::unique_ptr<Shader> m_shader;
};