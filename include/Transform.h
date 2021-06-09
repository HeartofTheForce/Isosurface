#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

const glm::vec3 WorldRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 WorldUp = glm::vec3(0.0f, 10.0f, 0.0f);
const glm::vec3 WorldForward = glm::vec3(0.0f, 0.0f, 1.0f);

class Transform
{
    bool _dirty{false};
    glm::mat4 _matrix{glm::mat4(1.0f)};

    glm::vec3 _position{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::quat _orientation{glm::quat(1.0f, 0.0f, 0.0f, 0.0f)};
    glm::vec3 _scale{glm::vec3(1.0f)};

  public:
    glm::vec3 Position() const
    {
        return _position;
    }
    void Position(const glm::vec3& newValue)
    {
        _position = newValue;
        _dirty = true;
    }

    glm::quat Orientation() const
    {
        return _orientation;
    }
    void Orientation(const glm::quat& newValue)
    {
        _orientation = newValue;
        _dirty = true;
    }

    glm::vec3 Scale() const
    {
        return _scale;
    }
    void Scale(const glm::vec3& newValue)
    {
        _scale = newValue;
        _dirty = true;
    }

    glm::vec3 Right() const
    {
        return _orientation * WorldRight;
    }
    glm::vec3 Up() const
    {
        return _orientation * WorldUp;
    }
    glm::vec3 Forward() const
    {
        return _orientation * WorldForward;
    }

    glm::mat4 GetMatrix();
};
