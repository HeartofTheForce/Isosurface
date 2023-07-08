#pragma once
#include <glm/glm.hpp>

struct IndexMap
{
    const glm::uvec3 Size;
    const int TotalSize;

    IndexMap(
        glm::uvec3 size)
        : Size(size),
          TotalSize(Size.x * Size.y * Size.z)
    {
    }

    inline glm::uvec3 Decode(int index)
        const
    {
        glm::uvec3 coord;

        coord.z = index / (Size.x * Size.y);
        index -= coord.z * Size.x * Size.y;
        coord.y = index / Size.x;
        coord.x = index % Size.x;

        return coord;
    }

    inline int Encode(const glm::uvec3& coord) const
    {
        return (coord.z * Size.x * Size.y) + (coord.y * Size.x) + coord.x;
    }
};
