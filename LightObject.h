#pragma once
#include "Light.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "Shader.h"
#include <memory>

#define GRAND_STAR_OBJ_PATH "assets/Grand_Star_Mario(for_light_source).obj"
#define GRAND_STAR_TEXTURE_PATH "" // No texture just white.

// Should have its own seperate shader that doesnt color it weird.
class LightObject {
public:
  LightObject(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
              glm::vec3 specular)
      : m_light(position, ambient, diffuse, specular),
        m_light_object(
            ObjLoader().load(GRAND_STAR_OBJ_PATH, GRAND_STAR_TEXTURE_PATH)) {};

  LightObject(glm::vec3 position) // with standard light presets
      : m_light(position, glm::vec3(0.15f), glm::vec3(0.8f), glm::vec3(1.0f)),
        m_light_object(
            ObjLoader().load(GRAND_STAR_OBJ_PATH, GRAND_STAR_TEXTURE_PATH)) {};

  glm::vec3 getPosition() const;
  bool isActive() const;
  void setActive(bool is_active);

  void draw(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &projection);
  void setLightUniforms(Shader &shader, int index) const;
  static void initShader(); // intit its static shader for light objects.

private:
  void setMVP(const glm::mat4 &model, const glm::mat4 &view,
              const glm::mat4 &projection);

  static std::unique_ptr<Shader> m_shader;
  Light m_light;
  Mesh m_light_object;
  bool m_is_active = true;
};
