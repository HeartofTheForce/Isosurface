#include <Graphics/Camera.h>

namespace
{
const float PitchLimit = glm::radians(90.0f);
const float SensitivtyFactor = 0.022f;

// float FovH2V(float hFov, float aspect)
// {
//     float vFov = 2 * glm::atan(glm::tan(hFov / 2) / aspect);
//     return vFov;
// }

// float FovV2H(float vFov, float aspect)
// {
//     float hFov = 2 * glm::atan(glm::tan(vFov / 2) * aspect);
//     return hFov;
// }
} // namespace

void Camera::Update(const float& speed, const glm::vec3& moveDirection, const float& mouseSensitivity, const glm::vec2& mouseOffset)
{
    _yaw += glm::radians(mouseOffset.x) * mouseSensitivity * SensitivtyFactor;
    _pitch += glm::radians(mouseOffset.y) * mouseSensitivity * SensitivtyFactor;

    if (_pitch >= PitchLimit)
        _pitch = PitchLimit;
    if (_pitch <= -PitchLimit)
        _pitch = -PitchLimit;

    Transform.Orientation(glm::quat(glm::vec3(_pitch, _yaw, 0.0f)));

    if (glm::dot(moveDirection, moveDirection) > 0)
    {
        auto wishDir = glm::normalize(moveDirection);
        Transform.Position(Transform.Position() + Transform.Orientation() * wishDir * speed);
    }
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::inverse(Transform.GetMatrix());
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(_fovY, _aspect, _near, _far);
}

Camera::Camera(float width, float height)
    : _aspect(width / height)
{
}
