#include <glm/ext/vector_float3.hpp>

class Light {
public:
    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular): m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {};

    glm::vec3 m_position;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};