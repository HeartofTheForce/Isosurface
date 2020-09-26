#include <Transform.h>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::GetMatrix()
{
    if (_dirty)
    {
        _matrix = glm::translate(glm::mat4(1.0f), _position);
        _matrix *= glm::mat4_cast(_orientation);
        _matrix = glm::scale(_matrix, _scale);

        _dirty = false;
    }

    return _matrix;
}
