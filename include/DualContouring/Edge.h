#pragma once
#include <memory>
#include <glm/glm.hpp>


struct EdgeMap
{
  const int SizeX{0};
  const int SizeY{0};
  const int SizeZ{0};
  const int TotalSize{0};

  std::unique_ptr<glm::vec3[]> Intersection = {nullptr};
  std::unique_ptr<float[]> Direction = {nullptr};
  std::unique_ptr<int[]> IndexOffset = {nullptr};

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
