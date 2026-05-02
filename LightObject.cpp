#include "LightObject.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

std::unique_ptr<Shader> LightObject::m_shader = nullptr;

void LightObject::initShader() {
  m_shader =
      std::make_unique<Shader>("shaders/light_object_vertex_shader.glsl",
                               "shaders/light_object_fragment_shader.glsl");
}

void LightObject::draw(const glm::mat4 &model, const glm::mat4 &view,
                       const glm::mat4 &projection) {
  m_shader->useProgram();

  setMVP(model, view, projection);

  m_light_object.draw();
}

void LightObject::setLightUniforms(Shader &shader, int index) const {
  std::string baseString = "lights[" + std::to_string(index) + "].";
  shader.setVec3Uniform((baseString + "position").c_str(), m_light.m_position);
  shader.setVec3Uniform((baseString + "ambient").c_str(), m_light.m_ambient);
  shader.setVec3Uniform((baseString + "diffuse").c_str(), m_light.m_diffuse);
  shader.setVec3Uniform((baseString + "specular").c_str(), m_light.m_specular);
}

void LightObject::setMVP(const glm::mat4 &model, const glm::mat4 &view,
                         const glm::mat4 &projection) {
  glm::mat4 grandStarModel =
      glm::translate(glm::mat4(1.0f), m_light.m_position);
  grandStarModel = glm::scale(grandStarModel, glm::vec3(0.02f));
  m_shader->setMat4("model", grandStarModel);
  m_shader->setMat4("view", view);
  m_shader->setMat4("projection", projection);
}
