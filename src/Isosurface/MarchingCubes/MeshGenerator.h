#pragma once
#include <Graphics/Meshes/MeshCpu.h>
#include <Isosurface/CachedSDF.h>

class MeshGenerator
{
    const std::shared_ptr<CachedSDF> _cachedSDF;

    const IndexMap Index;

    MeshCpu BuildMesh(const int& totalVertices);
  public:
    MeshCpu GenerateMesh();

    MeshGenerator(std::shared_ptr<CachedSDF> cachedSDF)
        : _cachedSDF(cachedSDF),
          Index(_cachedSDF->Index.SizeX - 1, _cachedSDF->Index.SizeY - 1, _cachedSDF->Index.SizeZ - 1)
    {
    }
};
