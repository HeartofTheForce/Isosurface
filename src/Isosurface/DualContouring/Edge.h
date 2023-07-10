#pragma once
#include <glm/glm.hpp>
#include <memory>

struct EdgeMap
{
    const IndexMap Index;

    const std::unique_ptr<glm::vec3[]> Intersections;
    const std::unique_ptr<glm::vec3[]> Normals;
    const std::unique_ptr<float[]> Directions;
    const std::unique_ptr<int[]> IndexOffset;

    EdgeMap(
        const glm::uvec3& size)
        : Index(size),
          Intersections(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])),
          Normals(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])),
          Directions(std::unique_ptr<float[]>(new float[Index.TotalSize])),
          IndexOffset(std::unique_ptr<int[]>(new int[Index.TotalSize]))
    {
    }
};
