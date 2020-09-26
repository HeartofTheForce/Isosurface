#pragma once
#include <memory>
#include <glm/glm.hpp>

struct Edge
{
  glm::vec3 Intersection;
  float Direction;
  int IndexOffset;

  Edge(
      const glm::vec3 &intersection,
      const float &direction,
      const int &indexOffset)
      : Intersection(intersection),
        Direction(direction),
        IndexOffset(indexOffset) {}

  Edge()
      : Intersection(glm::vec3(0.0f)),
        Direction(0.0f),
        IndexOffset(-1) {}
};

struct EdgeMap
{
  const int SizeX{0};
  const int SizeY{0};
  const int SizeZ{0};
  const int TotalSize{0};

  std::unique_ptr<Edge[]> Edges{nullptr};

  Edge &Get(const int &x, const int &y, const int &z) const;

  EdgeMap(
      const int &sizeX,
      const int &sizeY,
      const int &sizeZ)
      : SizeX(sizeX),
        SizeY(sizeY),
        SizeZ(sizeZ),
        TotalSize(SizeX * SizeY * SizeZ),
        Edges(std::unique_ptr<Edge[]>(new Edge[TotalSize])) {}

  EdgeMap(const EdgeMap &) = delete;
  EdgeMap &operator=(const EdgeMap &) = delete;
  EdgeMap(EdgeMap &&) = default;
  EdgeMap &operator=(EdgeMap &&) = default;
};
