#pragma once
#include <Graphics/Meshes/MeshCpu.h>
#include <Isosurface/CachedSDF.h>
#include <Isosurface/Common.h>
#include <Isosurface/DualContouring/Edge.h>

namespace DualContouring
{
class MeshGenerator
{
    const std::shared_ptr<CachedSDF> _cachedSDF;

    const IndexMap _index;

    const EdgeMap _edgeMaps[3];

    const std::unique_ptr<bool[]> _cubeCheck;
    const std::unique_ptr<glm::vec3[]> _cubeVertices;

    int _counter{0};

    MeshCpu BuildMesh(const int& totalVertices);
    inline void PopulateMesh(const int& index, MeshCpu& meshCpu);
    inline void PushEdge(
        MeshCpu& meshCpu,
        const int& offset,
        const float& dir,
        const glm::uvec3& shared0,
        const glm::uvec3& shared1,
        const glm::uvec3& unique0,
        const glm::uvec3& unique1
    );
    inline void CalculateEdge(int index);
    inline void CalculateEdge(const int iA, const glm::uvec3& coord, const float& d0, const glm::vec3& p0, const glm::vec3& n0);
    inline void CalculateVertex(int index);
    inline void AggregateEdge(
        const EdgeMap& edgeMap,
        const glm::uvec3& coord,
        glm::vec3& sum,
        glm::vec3* points,
        glm::vec3* normals,
        size_t& cnt
    );

  public:
    MeshCpu GenerateMesh();

    MeshGenerator(std::shared_ptr<CachedSDF> cachedSDF)
        : _cachedSDF(cachedSDF),
          _index(_cachedSDF->Index.Size - glm::uvec3(1)),
          // clang-format off
          _edgeMaps {
            EdgeMap{glm::uvec3(_index.Size.x, _cachedSDF->Index.Size.y, _cachedSDF->Index.Size.z)},
            EdgeMap{glm::uvec3(_cachedSDF->Index.Size.x, _index.Size.y, _cachedSDF->Index.Size.z)},
            EdgeMap{glm::uvec3(_cachedSDF->Index.Size.x, _cachedSDF->Index.Size.y, _index.Size.z)}
          },
          // clang-format on
          _cubeCheck(std::unique_ptr<bool[]>(new bool[_index.TotalSize])),
          _cubeVertices(std::unique_ptr<glm::vec3[]>(new glm::vec3[_index.TotalSize]))
    {
    }
};
} // namespace DualContouring