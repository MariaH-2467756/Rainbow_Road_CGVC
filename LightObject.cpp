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

  if (!m_is_active)
    return;

  m_shader->useProgram();
  setMVP(model, view, projection);
  m_light_object.draw();
}

glm::vec3 LightObject::getPosition() const { return m_light.m_position; }

bool LightObject::isActive() const { return m_is_active; }

void LightObject::setActive(bool is_active) { m_is_active = is_active; }

void LightObject::setLightUniforms(Shader &shader, int index) const {
  std::string base = "lights[" + std::to_string(index) + "].";
  glm::vec3 zero(0.0f);
  shader.setVec3Uniform((base + "position").c_str(), m_light.m_position);
  shader.setVec3Uniform((base + "ambient").c_str(),
                        m_is_active ? m_light.m_ambient : zero);
  shader.setVec3Uniform((base + "diffuse").c_str(),
                        m_is_active ? m_light.m_diffuse : zero);
  shader.setVec3Uniform((base + "specular").c_str(),
                        m_is_active ? m_light.m_specular : zero);
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
