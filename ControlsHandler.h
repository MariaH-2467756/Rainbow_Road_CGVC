#pragma once
#include "Camera.h"
#include "Window.h"

class ControlsHandler {
public:
  ControlsHandler(Camera &camera, Window &window, float width, float height);

  void processInput(float delta);
  void mouseCallback(double xpos_in, double ypos_in);
  void scrollCallback(double y_offset);

private:
  Camera &m_camera;
  Window &m_window;
  float m_lastX;
  float m_lastY;
  bool m_firstMouse = true;
};