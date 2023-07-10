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
} // namespace SDF