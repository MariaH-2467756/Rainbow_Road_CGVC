#pragma once
#include "FrameBuffer.h"
#include "PostProcessEffect.h"
#include "ScreenQuad.h"

class LaplacianEffect : public PostProcessEffect {
public:
  LaplacianEffect(unsigned int width, unsigned int height)
      : PostProcessEffect("shaders/laplacian_fragment_shader.glsl", width,
                          height),
        m_tempFboH(width, height), m_tempFboV(width, height) {}

  void addEffect(FrameBuffer &source, ScreenAllignedQuad &saq) override {
    m_shader.useProgram();

    m_tempFboH.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.setIntUniform("passIndex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.getTexture());
    saq.draw();
    m_tempFboH.unbind();

    m_tempFboV.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.setIntUniform("passIndex", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.getTexture());
    saq.draw();
    m_tempFboV.unbind();

    source.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.setIntUniform("passIndex", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tempFboH.getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_tempFboV.getTexture());
    saq.draw();
    source.unbind();
  }

private:
  FrameBuffer m_tempFboH;
  FrameBuffer m_tempFboV;
};