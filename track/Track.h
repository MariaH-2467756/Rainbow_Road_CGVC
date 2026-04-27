#include "TrackCurves.h"
#include <vector>

struct TrackVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Track {
public:
    Track(std::vector<BezierCurve> curves);
    void addCurve(const BezierCurve& curve, int steps = 50);
    void build(float width);

    std::vector<TrackVertex> vertices() const { return m_vertices; };
    int vertexCount() const { return m_vertices.size(); };
    glm::mat4 getTransformAtDistance(float d) const;
    float totalLength() const { return m_arcLength.back(); }


private:
    std::vector<glm::vec3> m_points;
    std::vector<TrackVertex> m_vertices;
    std::vector<float> m_arcLength;

};