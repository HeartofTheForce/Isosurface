#pragma once
#include <PerlinNoise.hpp>
#include <functional>
#include <glm/glm.hpp>

namespace SDF
{
typedef std::function<float(glm::vec3)> SDF;

SDF Box(glm::vec3 b);
SDF Sphere(float r);
SDF Translate(glm::vec3 offset, SDF f);

float Noise(glm::vec3 p);

glm::vec3 Normal(SDF f, glm::vec3 p, glm::vec3 eps);

SDF Intersect(SDF a, SDF b);
SDF Union(SDF a, SDF b);
SDF Difference(SDF a, SDF b);

} // namespace SDF