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

#include "CrosshairEffect.h"
#include "GaussianBlurEffect.h"
#include "LaplacianEffect.h"

#define NUM_LIGHTS 16
#define LIGHTS                                                                        \
  {                                                                                   \
    {{-10.0f, 15.0f, 5.0f}}, {{20.0f, 12.0f, -5.0f}}, {{15.0f, 25.0f, 20.0f}},        \
        {{0.0f, 18.0f, 25.0f}}, {{0.0f, 5.0f, 25.0f}},                                \
        {{-15.0f, -25.0f, 0.0f}}, {{10.0f, -45.0f, 5.0f}},                            \
        {{0.0f, -72.0f, -15.0f}}, {{35.0f, -20.0f, 35.0f}},                           \
        {{0.0f, 20.0f, 55.0f}}, {{55.0f, 50.0f, -10.0f}},                             \
        {{-55.0f, 45.0f, 25.0f}}, {{-45.0f, 20.0f, -20.0f}},                          \
        {{30.0f, 35.0f, -50.0f}}, {{-25.0f, 10.0f, -30.0f}}, {{-20.0f, 10.0f, 15.0f}} \
  }

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
  LightObject lights[] = LIGHTS;

  // matrices
  glm::mat4 model = glm::mat4(1.0f);
  // normalMatrix transforms normals correctly when model is scaled/rotated
  glm::mat3 normalMatrix =
      glm::mat3(glm::transpose(glm::inverse(model))); // Calc normal on CPU.

  // framebuffer logic
  FrameBuffer fbo = FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGTH);
  FrameBuffer bloomFbo = FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGTH);
  ScreenAllignedQuad::initShader();
  ScreenAllignedQuad saq = ScreenAllignedQuad();

  // make the effects
  GaussianBlurEffect gaussianBlur =
      GaussianBlurEffect(WINDOW_WIDTH, WINDOW_HEIGTH);
  LaplacianEffect laplacian = LaplacianEffect(WINDOW_WIDTH, WINDOW_HEIGTH);
  CrosshairEffect crosshair = CrosshairEffect(WINDOW_WIDTH, WINDOW_HEIGTH);

  glEnable(GL_DEPTH_TEST);

  // vars for moving calcs.
  float distanceTravelled = 0.0f;
  float kartSpeed = 15.0f;
  float lakituSpeed = 1.0f;
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

    // picking
    if (controls->wasLeftClicked()) {
      int picked = window.getPickedLight(lights, NUM_LIGHTS, view, proj);
      if (picked != -1)
        lights[picked].setActive(false);
    }

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
    for (int i = 0; i < NUM_LIGHTS; i++) {
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

    // pass kartmodel to controls for camera follow
    controls->setKartTransform(kartModel);

    glm::mat3 kartNormalMatrix =
        glm::mat3(glm::transpose(glm::inverse(kartModel)));

    shader.setMat3("normalMatrix", kartNormalMatrix);
    mario.draw(shader, kartModel, view, proj);
    lakituWithSign.draw(shader, kartModel, view, proj, lastTime * lakituSpeed);

    // draw the light objects (AKA grandstars)
    for (int i = 0; i < NUM_LIGHTS; i++) { // last, different shder.
      lights[i].draw(model, view, proj);
    }

    fbo.unbind();

    bloomFbo.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // take the depth from fbo and put in in the bloomfbo.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bloomFbo.getFBO());
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH, 0, 0, WINDOW_WIDTH,
                      WINDOW_HEIGTH, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, bloomFbo.getFBO());
    glDepthFunc(GL_EQUAL);

    // use depth test when drawing to bloomfbo
    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < NUM_LIGHTS; i++) {
      lights[i].draw(model, view, proj);
    }

    // reset the depth func.
    glDepthFunc(GL_LESS);
    bloomFbo.unbind();

    int mode = window.getPostProcessingState();
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint finalTex;
    GLuint bloomTex;

    switch (mode) {
    case 0:
      break;

    case 1:
      gaussianBlur.addEffect(fbo, saq);
      break;

    case 2:
      laplacian.addEffect(fbo, saq);
      break;

    case 3:
      gaussianBlur.addEffect(bloomFbo, saq);
      bloomTex = bloomFbo.getTexture();
      break;
    }

    if (window.getCrosshair()) {
      crosshair.addEffect(fbo, saq);
    }

    finalTex = fbo.getTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    saq.draw(finalTex);

    if (mode == 3) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE);
      saq.draw(bloomTex);
      glDisable(GL_BLEND);
    }

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
