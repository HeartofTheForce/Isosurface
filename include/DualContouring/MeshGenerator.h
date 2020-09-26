#pragma once
#include <DualContouring/CachedSDF.h>
#include <DualContouring/Edge.h>
#include <Graphics/Meshes/MeshCpu.h>

class MeshGenerator
{
    const CachedSDF *_cachedSDF;

    const int _sizeX;
    const int _sizeY;
    const int _sizeZ;

    const int _totalSize;

    const EdgeMap _edgeMapX;
    const EdgeMap _edgeMapY;
    const EdgeMap _edgeMapZ;

    const std::unique_ptr<bool[]> _cubeCheck;
    const std::unique_ptr<glm::vec3[]> _cubeVertices;

    int _counter{0};

    MeshCpu BuildMesh(const int &totalVertices);
    void PopulateMesh(const int &index, MeshCpu &meshCpu);
    void CalculateEdge(int index);
    void CalculateEdgeX(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    void CalculateEdgeY(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    void CalculateEdgeZ(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0);
    void CalculateVertex(int index);
    void AggregateEdgeX(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);
    void AggregateEdgeY(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);
    void AggregateEdgeZ(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt);

public:
    MeshCpu GenerateMesh();

    MeshGenerator(CachedSDF *cachedSDF)
        : _cachedSDF(cachedSDF),
          _sizeX(_cachedSDF->SizeX - 1),
          _sizeY(_cachedSDF->SizeY - 1),
          _sizeZ(_cachedSDF->SizeZ - 1),
          _totalSize(_sizeX * _sizeY * _sizeZ),
          _edgeMapX(EdgeMap{_sizeX, _cachedSDF->SizeY, _cachedSDF->SizeZ}),
          _edgeMapY(EdgeMap{_cachedSDF->SizeX, _sizeY, _cachedSDF->SizeZ}),
          _edgeMapZ(EdgeMap{_cachedSDF->SizeX, _cachedSDF->SizeY, _sizeZ}),
          _cubeCheck(std::unique_ptr<bool[]>(new bool[_totalSize])),
          _cubeVertices(std::unique_ptr<glm::vec3[]>(new glm::vec3[_sizeX * _sizeY * _sizeZ])) {}

    MeshGenerator(const MeshGenerator &) = delete;
    MeshGenerator &operator=(const MeshGenerator &) = delete;
};
