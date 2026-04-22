#include <glm/glm.hpp>

class BezierCurve {
public:
    BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3): m_p0(p0), m_p1(p1), m_p2(p2), m_p3(p3) {}
    std::vector<glm::vec3> drawCurve(int steps) const;

private:
    glm::vec3 m_p0, m_p1, m_p2, m_p3;
};