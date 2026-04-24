#include "Track.h"
#include <vector>

Track::Track(std::vector<BezierCurve> curves)
{
    for (BezierCurve curve : curves){
        addCurve(curve);
    }
}

void Track::addCurve(const BezierCurve& curve, int steps){
    std::vector<glm::vec3> pts = curve.drawCurve(steps);
    
    // skip first point of each new curve (it's the same as last point)
    int start = m_points.empty() ? 0 : 1;
    for (int i = start; i < pts.size(); i++)
        m_points.push_back(pts[i]);
}

void Track::build(float width) {
    m_vertices.clear();
    m_arcLength.clear();
    m_arcLength.push_back(0.0f);

    for (int i = 0; i < (int)m_points.size(); i++) {
        glm::vec3 tangent;
        if (i == 0) { tangent = m_points[1] - m_points[0]; }
        else if (i == (int)m_points.size() - 1) { tangent = m_points[i] - m_points[i-1]; }
        else { tangent = m_points[i+1] - m_points[i-1]; }

        tangent = glm::normalize(tangent);
        glm::vec3 side   = glm::normalize(glm::cross(tangent, glm::vec3(0,1,0)));
        glm::vec3 normal = glm::normalize(glm::cross(tangent, side)); // points up

        float u = (float)i / (float)(m_points.size() - 1);
        m_vertices.push_back({ m_points[i] + side * width, normal, { u, 1.f } });
        m_vertices.push_back({ m_points[i] - side * width, normal, { u, 0.f } });

        if (i > 0) { m_arcLength.push_back(m_arcLength.back() + glm::length(m_points[i] - m_points[i-1])); }
    }
}

glm::mat4 Track::getTransformAtDistance(float d) const {
    float total = m_arcLength.back();
    // wrap around for a closed loop
    d = fmod(d, total);
    if (d < 0) d += total;

    // Binary search for the segment containing d
    int hi = (int)(std::lower_bound(m_arcLength.begin(), m_arcLength.end(), d)
                   - m_arcLength.begin());
    if (hi == 0) hi = 1;
    if (hi >= (int)m_arcLength.size()) hi = (int)m_arcLength.size() - 1;
    int lo = hi - 1;

    // Interpolate position
    float segLen = m_arcLength[hi] - m_arcLength[lo];
    float t      = (segLen > 1e-6f) ? (d - m_arcLength[lo]) / segLen : 0.0f;
    glm::vec3 pos = glm::mix(m_points[lo], m_points[hi], t);

    // Build orientation from tangent
    glm::vec3 tangent = glm::normalize(m_points[hi] - m_points[lo]);
    glm::vec3 up      = glm::vec3(0, 1, 0);
    glm::vec3 right   = glm::normalize(glm::cross(tangent, up));
    up                = glm::normalize(glm::cross(right, tangent)); // reorthogonalize

    // Build a 4x4 model matrix: [right | up | -tangent | pos]
    // -tangent because in OpenGL forward is -Z by convention
    glm::mat4 m(1.0f);
    m[0] = glm::vec4(right,    0);
    m[1] = glm::vec4(up,       0);
    m[2] = glm::vec4(-tangent, 0);
    m[3] = glm::vec4(pos,      1);
    return m;
}