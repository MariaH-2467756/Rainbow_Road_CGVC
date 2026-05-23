#include "Window.h"

Window::Window(int width, int height, const char *name)
    : m_width(width), m_height(height), m_name(name), m_postProcessingState(0),
      m_crosshair(false) {
  init();
}

Window::~Window() { close(); }

GLFWwindow *Window::getGLFWwindow() const { return m_window; }

bool Window::shouldClose() { return glfwWindowShouldClose(m_window); }

void Window::swapBuffers() { glfwSwapBuffers(m_window); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::setupCallbacks(GLFWcursorposfun mouseCallback,
                            GLFWscrollfun scrollCallback) {
  // correctly resize window
  glfwSetFramebufferSizeCallback(m_window,
                                 [](GLFWwindow *w, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(m_window, mouseCallback);
  glfwSetScrollCallback(m_window, scrollCallback);
}

int Window::getPickedLight(LightObject lights[], int count,
                           const glm::mat4 &view, const glm::mat4 &proj,
                           float thresholdNDC) {
  int best = -1;
  float bestDist = thresholdNDC;

  for (int i = 0; i < count; i++) {
    if (!lights[i].isActive())
      continue;

    glm::vec4 clip = proj * view * glm::vec4(lights[i].getPosition(), 1.0f);
    if (clip.w <= 0.0f)
      continue;

    glm::vec2 ndc = glm::vec2(clip.x, clip.y) / clip.w;
    float dist = glm::length(ndc);

    if (dist < bestDist) {
      bestDist = dist;
      best = i;
    }
  }
  return best;
}

void Window::init() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
  }

  // Tell it what version of openGL I'll we using (3.3 with core profile.)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create our window
  m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);

  // Check if the window was created
  if (!m_window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate(); // If failed we use GLFW to terminate.
    return;
  }

  // Put our window in the context.
  glfwMakeContextCurrent(m_window);
}

void Window::close() {
  // Clean up the window;
  glfwDestroyWindow(m_window);
  glfwTerminate();
}