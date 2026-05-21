#include "ControlsHandler.h"

ControlsHandler::ControlsHandler(Camera &camera, Window &window, float width,
                                 float height)
    : m_camera(camera), m_window(window), m_lastX(width / 2.0f),
      m_lastY(height / 2.0f) {}

// keyboard controls
void ControlsHandler::processInput(float delta) {
  GLFWwindow *glfw_window = m_window.getGLFWwindow();
  // exit window
  if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(glfw_window, true);

  // post - processing state.
  if (glfwGetKey(glfw_window, GLFW_KEY_N) == GLFW_PRESS)
    m_window.setPostProcessingState(0);

  if (glfwGetKey(glfw_window, GLFW_KEY_G) == GLFW_PRESS)
    m_window.setPostProcessingState(1);

  if (glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS)
    m_window.setPostProcessingState(2);

  if (glfwGetKey(glfw_window, GLFW_KEY_B) == GLFW_PRESS)
    m_window.setPostProcessingState(3);

  if (glfwGetKey(glfw_window, GLFW_KEY_C) == GLFW_PRESS)
    m_window.toggleCrosshair();

  // on tab press: toggle followTarger
  bool tabDown = glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_PRESS;
  if (tabDown && !m_tabWasPressed)
    m_followKart = !m_followKart;
  m_tabWasPressed = tabDown;

  // camera controls
  if (!m_followKart) {
    if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
      m_camera.ProcessKeyboard(FORWARD, delta);
    if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
      m_camera.ProcessKeyboard(BACKWARD, delta);
    if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
      m_camera.ProcessKeyboard(LEFT, delta);
    if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
      m_camera.ProcessKeyboard(RIGHT, delta);
    if (glfwGetKey(glfw_window, GLFW_KEY_SPACE) == GLFW_PRESS)
      m_camera.ProcessKeyboard(UP, delta);
    if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      m_camera.ProcessKeyboard(DOWN, delta);
  } else {
    m_camera.FollowTarget(m_kartTransform);
  }
}

// mouse movement controls
void ControlsHandler::mouseCallback(double xpos_in, double ypos_in) {
  float xpos = static_cast<float>(xpos_in);
  float ypos = static_cast<float>(ypos_in);

  if (m_firstMouse) {
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = false;
  }

  if (!m_followKart) {
    float xoffset = xpos - m_lastX;
    float yoffset =
        m_lastY - ypos; // reversed since y-coordinates go from bottom to top
    m_camera.ProcessMouseMovement(xoffset, yoffset);
  }
  m_lastX = xpos;
  m_lastY = ypos;
}

// mouse scroll controls
void ControlsHandler::scrollCallback(double y_offset) {
  m_camera.ProcessMouseScroll(static_cast<float>(y_offset));
}

void ControlsHandler::setKartTransform(const glm::mat4 &t) {
  m_kartTransform = t;
}
bool ControlsHandler::isFollowingKart() const { return m_followKart; }