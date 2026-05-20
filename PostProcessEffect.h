#pragma once
#include "Shader.h"
#include <functional>
#include <string>

class FrameBuffer;
class ScreenAllignedQuad;

class PostProcessEffect {
public:
  PostProcessEffect(const char *fragementShaderPath, unsigned int width,
                    unsigned int height);

  void virtual addEffect(FrameBuffer &buffer, ScreenAllignedQuad &saq) = 0;

protected:
  Shader m_shader;
  unsigned int m_width, m_height;
};