#pragma once
#include <DualContouring/CachedSDF.h>
#include <DualContouring/Edge.h>
#include <Graphics/Meshes/MeshCpu.h>

class MeshGenerator
{
    const CachedSDF *_cachedSDF;

    const IndexMap Index;

    const EdgeMap _edgeMapX;
    const EdgeMap _edgeMapY;
    const EdgeMap _edgeMapZ;

    const std::unique_ptr<bool[]> _cubeCheck;
    const std::unique_ptr<glm::vec3[]> _cubeVertices;

    int _counter{0};

    MeshCpu BuildMesh(const int &totalVertices);
    inline void PopulateMesh(const int &index, MeshCpu &meshCpu);
    inline void CalculateEdge(int index);
    inline void CalculateEdgeX(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    inline void CalculateEdgeY(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    inline void CalculateEdgeZ(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    inline void CalculateVertex(int index);
    inline void AggregateEdge(
        const EdgeMap &edgeMap,
        const int &x,
        const int &y,
        const int &z,
        glm::vec3 &sum,
        int &cnt);
    inline void AggregateEdgeX(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);
    inline void AggregateEdgeY(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);
    inline void AggregateEdgeZ(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);

public:
    MeshCpu GenerateMesh();

    MeshGenerator(CachedSDF *cachedSDF)
        : _cachedSDF(cachedSDF),
          Index(_cachedSDF->Index.SizeX - 1, _cachedSDF->Index.SizeY - 1, _cachedSDF->Index.SizeZ - 1),
          _edgeMapX(EdgeMap{Index.SizeX, _cachedSDF->Index.SizeY, _cachedSDF->Index.SizeZ}),
          _edgeMapY(EdgeMap{_cachedSDF->Index.SizeX, Index.SizeY, _cachedSDF->Index.SizeZ}),
          _edgeMapZ(EdgeMap{_cachedSDF->Index.SizeX, _cachedSDF->Index.SizeY, Index.SizeZ}),
          _cubeCheck(std::unique_ptr<bool[]>(new bool[Index.TotalSize])),
          _cubeVertices(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])) {}

    MeshGenerator(const MeshGenerator &) = delete;
    MeshGenerator &operator=(const MeshGenerator &) = delete;
};
