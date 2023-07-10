#pragma once
#include <glm/glm.hpp>

namespace
{
inline glm::vec3 Interpolate(const float& isoLevel, const glm::vec3& p1, const glm::vec3& p2, const float& valp1, const float& valp2)
{
    if (abs(valp2 - valp1) < 0.00001)
        return p1;

    float mu = (isoLevel - valp1) / (valp2 - valp1);
    return p1 + (p2 - p1) * mu;
}

inline float NonZeroSign(float v)
{
    float output = glm::sign(v);
    if (output == 0)
        return 1;
    else
        return output;
}
} // namespace