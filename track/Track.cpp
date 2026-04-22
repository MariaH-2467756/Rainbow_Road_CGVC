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
    }
}