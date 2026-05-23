#pragma once
#include "Camera.h"
#include "Window.h"

class ControlsHandler {
public:
  ControlsHandler(Camera &camera, Window &window, float width, float height);

  void processInput(float delta);
  bool wasLeftClicked();
  void mouseCallback(double xpos_in, double ypos_in);
  void scrollCallback(double y_offset);
  void setKartTransform(const glm::mat4 &t);
  bool isFollowingKart() const;
  void setLights(LightObject *lights, int count);

private:
  Camera &m_camera;
  Window &m_window;
  float m_lastX;
  float m_lastY;
  bool m_firstMouse = true;
  bool m_followKart = false;
  bool m_tab_WasPressed = false;
  bool m_c_wasPressed = false;
  bool m_r_wasPressed = false;
  bool m_lmb_WasPressed = false;
  glm::mat4 m_kartTransform = glm::mat4(1.0f);
  LightObject *m_lights = nullptr;
  int m_numLights = 0;
};