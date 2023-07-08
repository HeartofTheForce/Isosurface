#pragma once
#include <Transform.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    glm::vec3 _right{glm::vec3(0.0f)};
    glm::vec3 _up{glm::vec3(0.0f)};
    glm::vec3 _forward{glm::vec3(0.0f)};

    float _pitch{glm::radians(0.0f)};
    float _yaw{glm::radians(0.0f)};

    float _fovY{glm::radians(83.5817f)};
    float _aspect;
    float _near{0.1f};
    float _far{1000.0f};

  public:
    Transform Transform{};

    void Update(const float& speed, const glm::vec3& moveDirection, const float& mouseSensitivity, const glm::vec2& mouseOffset);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix() const;

    Camera(float width, float height);
};
