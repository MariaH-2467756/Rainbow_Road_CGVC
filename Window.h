#pragma once
#include "LightObject.h"
#include "includes/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <iostream>

class Window {
public:
  Window(int width, int height, const char *name);
  ~Window();
  GLFWwindow *getGLFWwindow() const;
  bool shouldClose();
  void swapBuffers();
  void pollEvents();
  void setupCallbacks(GLFWcursorposfun mouseCallback,
                      GLFWscrollfun scrollCallback);

  void setPostProcessingState(int state) { m_postProcessingState = state; };
  int getPostProcessingState() { return m_postProcessingState; };
  void toggleCrosshair() { m_crosshair = not m_crosshair; };
  bool getCrosshair() { return m_crosshair; }
  int getPickedLight(LightObject lights[], int count, const glm::mat4 &view,
                     const glm::mat4 &proj, float thresholdNDC = 0.08f);

private:
  void init();
  void close();

  int m_width, m_height;
  const char *m_name;
  GLFWwindow *m_window;
  int m_postProcessingState;
  bool m_crosshair;
};