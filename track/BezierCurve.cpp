#include "BezierCurve.h"
#include <vector>


std::vector<glm::vec3> BezierCurve::drawCurve(int steps) const
{
    std::vector<glm::vec3> points(steps+1);

    float stepSize = 1.0f/steps;
    float stepSizex2 = stepSize * stepSize;
    float stepSizex3 = stepSizex2 * stepSize;

    glm::vec3 p_t = -3.0f*m_p0 + 3.0f*m_p1;
    glm::vec3 p_t2 =  3.0f*m_p0 - 6.0f*m_p1 + 3.0f*m_p2;
    glm::vec3 p_t3 = -m_p0 + 3.0f*m_p1 - 3.0f*m_p2 + m_p3;

    glm::vec3 point  = m_p0; // Start bij p0
    glm::vec3 delta1 = p_t3*stepSizex3 + p_t2*stepSizex2 + p_t*stepSize;
    glm::vec3 delta2 = 6.0f*p_t3*stepSizex3 + 2.0f*p_t2*stepSizex2;
    glm::vec3 delta3 = 6.0f*p_t3*stepSizex3;

    for (int i = 0 ; i  <= steps ; i++){
        points[i] = point;
        point += delta1;
        delta1 += delta2;
        delta2 += delta3;
    }

    return points;
}