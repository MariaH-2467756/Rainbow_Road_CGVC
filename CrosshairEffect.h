#pragma once
#include "FrameBuffer.h"
#include "PostProcessEffect.h"
#include "ScreenQuad.h"

class CrosshairEffect : public PostProcessEffect {
public:
  CrosshairEffect(unsigned int width, unsigned int height)
      : PostProcessEffect("shaders/crosshair_fragments_shader.glsl", width,
                          height) {}

  void addEffect(FrameBuffer &fbo, ScreenAllignedQuad &saq) {
    fbo.bind();
    m_shader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.getTexture());
    saq.draw();
    fbo.unbind();
  }
};