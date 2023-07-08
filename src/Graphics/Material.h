#pragma once
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 Diffuse{glm::vec3(0.0f)};
    glm::vec3 Specular{glm::vec3(0.0f)};
    float Shininess{1.0f};
};
