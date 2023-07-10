#include <Isosurface/SDF.h>

namespace SDF
{
SDF Box(glm::vec3 b)
{
    return [b](glm::vec3 p) {
        glm::vec3 q = glm::vec3(glm::abs(p.x), glm::abs(p.y), glm::abs(p.z)) - b;
        glm::vec3 qMax = glm::vec3(glm::max(q.x, 0.0f), glm::max(q.y, 0.0f), glm::max(q.z, 0.0f));
        return glm::length(qMax) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.0f);
    };
}

SDF Sphere(float r)
{
    return [r](glm::vec3 p) {
        return glm::length(p) - r;
    };
}

SDF Translate(glm::vec3 offset, SDF f)
{
    return [offset, f](glm::vec3 p) {
        p -= offset;
        return f(p);
    };
}

int32_t octave = 2;
float scale = 0.175f;

siv::PerlinNoise perlin(111);
float Noise(glm::vec3 p)
{
    p = p * scale;
    return perlin.normalizedOctaveNoise3D(p.x, p.y, p.z, octave);
}
} // namespace SDF