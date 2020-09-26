#include <Graphics/Camera.h>

namespace
{
    const float PitchLimit = glm::radians(90.0f);
    const float SensitivtyFactor = 0.022f;

    // float FovHorizontalToVertical(float fovH, float aspect)
    // {
    //     float targetFovH = glm::radians(fovH);
    //     float targetAspect = aspect;
    //     float fovY = 2 * glm::atan(glm::tan(targetFovH / 2) * 1 / targetAspect);
    //     return fovY;
    // }
} // namespace

void Camera::Update(const float &speed, const glm::vec3 &moveDirection, const float &mouseSensitivity, const glm::vec2 &mouseOffset)
{
    _yaw += glm::radians(mouseOffset.x) * mouseSensitivity * SensitivtyFactor;
    _pitch += glm::radians(mouseOffset.y) * mouseSensitivity * SensitivtyFactor;

    Transform.Orientation(glm::quat(glm::vec3(_pitch, _yaw, 0.0f)));

    if (_pitch >= PitchLimit)
        _pitch = PitchLimit - FLT_EPSILON;
    if (_pitch <= -PitchLimit)
        _pitch = -PitchLimit + FLT_EPSILON;

    Transform.Position(Transform.Position() + (speed * Transform.Forward() * moveDirection.z));
    Transform.Position(Transform.Position() + (speed * Transform.Right() * moveDirection.x));
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::inverse(Transform.GetMatrix());
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(_fovY, _aspect, _near, _far);
}

Camera::Camera(float width, float height) : _aspect(width / height)
{
}
