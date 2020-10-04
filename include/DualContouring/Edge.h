#pragma once
#include <memory>
#include <glm/glm.hpp>


struct EdgeMap
{
  const int SizeX;
  const int SizeY;
  const int SizeZ;
  const int TotalSize;

  std::unique_ptr<glm::vec3[]> Intersection;
  std::unique_ptr<float[]> Direction;
  std::unique_ptr<int[]> IndexOffset;

  EdgeMap(
      const int &sizeX,
      const int &sizeY,
      const int &sizeZ)
      : SizeX(sizeX),
        SizeY(sizeY),
        SizeZ(sizeZ),
        TotalSize(SizeX * SizeY * SizeZ),
        Intersection(std::unique_ptr<glm::vec3[]>(new glm::vec3[TotalSize])),
        Direction(std::unique_ptr<float[]>(new float[TotalSize])),
        IndexOffset(std::unique_ptr<int[]>(new int[TotalSize])) {}

  EdgeMap(const EdgeMap &) = delete;
  EdgeMap &operator=(const EdgeMap &) = delete;
  EdgeMap(EdgeMap &&) = default;
  EdgeMap &operator=(EdgeMap &&) = default;
};
