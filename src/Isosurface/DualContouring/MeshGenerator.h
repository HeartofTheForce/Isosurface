#pragma once
#include <Graphics/Meshes/MeshCpu.h>
#include <Isosurface/CachedSDF.h>
#include <Isosurface/Common.h>
#include <Isosurface/DualContouring/Edge.h>

class MeshGenerator
{
    const std::shared_ptr<CachedSDF> _cachedSDF;

    const IndexMap Index;

    const EdgeMap _edgeMapX;
    const EdgeMap _edgeMapY;
    const EdgeMap _edgeMapZ;

    const std::unique_ptr<bool[]> _cubeCheck;
    const std::unique_ptr<glm::vec3[]> _cubeVertices;

    int _counter{0};

    MeshCpu BuildMesh(const int& totalVertices);
    inline void PopulateMesh(const int& index, MeshCpu& meshCpu);
    inline void PushEdge(
        MeshCpu& meshCpu,
        const int& offset,
        const float& dir,
        const glm::ivec3& shared0,
        const glm::ivec3& shared1,
        const glm::ivec3& unique0,
        const glm::ivec3& unique1
    );
    inline void CalculateEdge(int index);
    inline void CalculateEdgeX(const glm::ivec3& coord, const float& d0, const glm::vec3& p0);
    inline void CalculateEdgeY(const glm::ivec3& coord, const float& d0, const glm::vec3& p0);
    inline void CalculateEdgeZ(const glm::ivec3& coord, const float& d0, const glm::vec3& p0);
    inline void CalculateVertex(int index);
    inline void AggregateEdge(
        const EdgeMap& edgeMap, const glm::ivec3& coord, glm::vec3& sum, int& cnt
    );

  public:
    MeshCpu GenerateMesh();

    MeshGenerator(std::shared_ptr<CachedSDF> cachedSDF)
        : _cachedSDF(cachedSDF), Index(_cachedSDF->Index.Size - glm::ivec3(1)),
          _edgeMapX(EdgeMap{
              glm::ivec3(Index.Size.x, _cachedSDF->Index.Size.y, _cachedSDF->Index.Size.z)}),
          _edgeMapY(EdgeMap{
              glm::ivec3(_cachedSDF->Index.Size.x, Index.Size.y, _cachedSDF->Index.Size.z)}),
          _edgeMapZ(EdgeMap{
              glm::ivec3(_cachedSDF->Index.Size.x, _cachedSDF->Index.Size.y, Index.Size.z)}),
          _cubeCheck(std::unique_ptr<bool[]>(new bool[Index.TotalSize])),
          _cubeVertices(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize]))
    {
    }
};
