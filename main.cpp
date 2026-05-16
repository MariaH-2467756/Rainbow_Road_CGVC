#include "Camera.h"
#include "ControlsHandler.h"
#include "LakituWithSign.h"
#include "LightObject.h"
#include "MarioInKart.h"
#include "Shader.h"
#include "Window.h"
#include "includes/GLFW/glfw3.h"
#include "includes/glad/glad.h"
#include "track/TrackRenderer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FrameBuffer.h"
#include "ScreenQuad.h"

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGTH = 800;
const char *WINDOW_NAME = "Rainbow Road";

// function prototypes
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// controlshandler init
ControlsHandler *controls = nullptr;

float delta = 0;

int main() {
  Window window = Window(WINDOW_WIDTH, WINDOW_HEIGTH, WINDOW_NAME);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // camera init
  Camera camera(glm::vec3(0.0f, 0.0f, 150.0f));
  // controlshandler declaration
  ControlsHandler controlsInstance(camera, window, WINDOW_WIDTH, WINDOW_HEIGTH);
  controls = &controlsInstance;

  window.setupCallbacks(mouse_callback, scroll_callback);

  Track track(getCurves()); // Test track
  track.build(1.6f);
  TrackRenderer trackRenderer;
  trackRenderer.upload(track, "assets/rainbow_road_texture.png");

  Shader shader = Shader("shaders/basic_vertex_shader.glsl",
                         "shaders/basic_fragment_shader.glsl");

  MarioInKart mario = MarioInKart();

  ChromaObject::initShader();
  LakituWithSign lakituWithSign = LakituWithSign();

  LightObject::initShader();
  LightObject lights[5] = {
      {glm::vec3(10.0f, 20.0f, 10.0f), glm::vec3(0.15f), glm::vec3(0.8f),
       glm::vec3(1.0f)},
      {glm::vec3(-50.0f, 30.0f, 0.0f), glm::vec3(0.15f),
       glm::vec3(0.6f, 0.0f, 0.6f), glm::vec3(1.0f, 0.0f, 1.0f)},
      {glm::vec3(0.0f, 50.0f, -100.0f), glm::vec3(0.15f),
       glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.8f, 0.2f)},
      {glm::vec3(100.0f, 15.0f, 50.0f), glm::vec3(0.15f),
       glm::vec3(0.0f, 0.4f, 0.8f), glm::vec3(0.0f, 0.6f, 1.0f)},
      {glm::vec3(-30.0f, 10.0f, 80.0f), glm::vec3(0.15f), glm::vec3(0.5f),
       glm::vec3(0.5f)}};

  // matrices
  glm::mat4 model = glm::mat4(1.0f);
  // normalMatrix transforms normals correctly when model is scaled/rotated
  glm::mat3 normalMatrix =
      glm::mat3(glm::transpose(glm::inverse(model))); // Calc normal on CPU.

  // framebuffer logic
  FrameBuffer fbo = FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGTH);
  FrameBuffer bloomFbo = FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGTH);
  ScreenAllignedQuad saq = ScreenAllignedQuad();
  Shader fboShader = Shader("shaders/post_processing_vertex_shader.glsl",
                            "shaders/post_processing_fragment_shader.glsl");

  glEnable(GL_DEPTH_TEST);

  // vars for moving calcs.
  float distanceTravelled = 0.0f;
  float kartSpeed = 15.0f;
  float lakituSpeed = 0.5f;
  float lastTime = glfwGetTime();

  while (!window.shouldClose()) {
    float now = glfwGetTime();
    delta = now - lastTime;
    controls->processInput(delta);

    // recalculate view and projection each frame
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.GetZoom()),
                                      static_cast<float>(WINDOW_WIDTH) /
                                          static_cast<float>(WINDOW_HEIGTH),
                                      0.1f, 1000.0f);

    // teken alles in fbo.
    fbo.bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.useProgram();

    // matrices uniforms
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", proj);
    shader.setMat3("normalMatrix", normalMatrix);

    // set the light uniforms
    for (int i = 0; i < 5; i++) {
      lights[i].setLightUniforms(shader, i);
    }

    // material uniforms
    shader.setVec3Uniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3Uniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.f));
    shader.setVec3Uniform("material.specular", glm::vec3(0.5f));
    shader.setFloatUniform("material.shininess", 32.0f);

    // draw track
    trackRenderer.draw();

    // draw objects moving allong track.
    lastTime = now;
    distanceTravelled += kartSpeed * delta;
    glm::mat4 kartModel = track.getTransformAtDistance(distanceTravelled);
    glm::mat3 kartNormalMatrix =
        glm::mat3(glm::transpose(glm::inverse(kartModel)));

    shader.setMat3("normalMatrix", kartNormalMatrix);
    mario.draw(shader, kartModel, view, proj);
    lakituWithSign.draw(shader, kartModel, view, proj, lastTime * lakituSpeed);

    // draw the light objects (AKA grandstars)
    for (int i = 0; i < 5; i++) { // last, different shder.
      lights[i].draw(model, view, proj);
    }

    fbo.unbind();

    bloomFbo.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 5; i++) {
      lights[i].draw(model, view, proj);
    }
    bloomFbo.unbind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    fboShader.useProgram();

    // Bind de textuur van het framebuffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());
    fboShader.setIntUniform("screenTexture", 0);
    fboShader.setVec2Uniform(
        "texelSize", glm::vec2((1.0f / WINDOW_WIDTH), (1.0f / WINDOW_HEIGTH)));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomFbo.GetTexture());
    fboShader.setIntUniform("bloomTexture", 1);
    fboShader.setIntUniform("effectMode", window.getPostPRoceesingState());

    saq.Draw();

    glEnable(GL_DEPTH_TEST);

    window.swapBuffers();
    window.pollEvents();
  }
}

// mouse controls wrapper functions
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  controls->mouseCallback(xpos, ypos);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  controls->scrollCallback(yoffset);
}
