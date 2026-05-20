#include "PostProcessEffect.h"

#define VERTEX_SHADER_PATH "shaders/post_processing_vertex_shader.glsl"

PostProcessEffect::PostProcessEffect(const char *fragementShaderPath,
                                     unsigned int width, unsigned int height)
    : m_shader(VERTEX_SHADER_PATH, fragementShaderPath), m_width(width),
      m_height(height) {}
