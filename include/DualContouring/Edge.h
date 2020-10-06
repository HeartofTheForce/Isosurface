#pragma once
#include <memory>
#include <glm/glm.hpp>

struct EdgeMap
{
  const IndexMap Index;

  std::unique_ptr<glm::vec3[]> Intersection;
  std::unique_ptr<float[]> Direction;
  std::unique_ptr<int[]> IndexOffset;

  EdgeMap(
      const int &sizeX,
      const int &sizeY,
      const int &sizeZ)
      : Index(sizeX, sizeY, sizeZ),
        Intersection(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])),
        Direction(std::unique_ptr<float[]>(new float[Index.TotalSize])),
        IndexOffset(std::unique_ptr<int[]>(new int[Index.TotalSize])) {}

  EdgeMap(const EdgeMap &) = delete;
  EdgeMap &operator=(const EdgeMap &) = delete;
  EdgeMap(EdgeMap &&) = default;
  EdgeMap &operator=(EdgeMap &&) = default;
};
