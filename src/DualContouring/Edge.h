#pragma once
#include <glm/glm.hpp>
#include <memory>

struct EdgeMap
{
    const IndexMap Index;

    const std::unique_ptr<glm::vec3[]> Intersection;
    const std::unique_ptr<float[]> Direction;
    const std::unique_ptr<int[]> IndexOffset;

    EdgeMap(
        const int& sizeX,
        const int& sizeY,
        const int& sizeZ)
        : Index(sizeX, sizeY, sizeZ),
          Intersection(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])),
          Direction(std::unique_ptr<float[]>(new float[Index.TotalSize])),
          IndexOffset(std::unique_ptr<int[]>(new int[Index.TotalSize]))
    {
    }
};
