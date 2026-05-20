#pragma once
#include "FrameBuffer.h"
#include "PostProcessEffect.h"
#include "ScreenQuad.h"

class GaussianBlurEffect : public PostProcessEffect {
public:
  GaussianBlurEffect(unsigned int width, unsigned int height)
      : PostProcessEffect("shaders/gaussian_blur_fragment_shader.glsl", width,
                          height),
        m_inbetweenFbo(width, height) {}

  void addEffect(FrameBuffer &source, ScreenAllignedQuad &saq) {
    m_inbetweenFbo.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.useProgram();
    m_shader.setIntUniform("passIndex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.getTexture());
    saq.draw();
    m_inbetweenFbo.unbind();

    source.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.setIntUniform("passIndex", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_inbetweenFbo.getTexture());
    saq.draw();
    source.unbind();
  }

private:
  FrameBuffer m_inbetweenFbo;
};