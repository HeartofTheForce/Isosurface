#pragma once
#include <Graphics/Meshes/MeshCpu.h>
#include <Isosurface/CachedSDF.h>
#include <Isosurface/Common.h>
#include <optional>

class MeshData
{
  public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    MeshData()
        : vertices(std::vector<glm::vec3>()),
          normals(std::vector<glm::vec3>())
    {
        this->vertices = std::vector<glm::vec3>();
        this->normals = std::vector<glm::vec3>();
    }

    MeshData(int capacity)
        : MeshData()
    {
        this->vertices.reserve(capacity);
        this->normals.reserve(capacity);
    }

    static void Merge(MeshData& a, const MeshData b)
    {
        a.vertices.insert(a.vertices.end(), b.vertices.begin(), b.vertices.end());
        a.normals.insert(a.normals.end(), b.normals.begin(), b.normals.end());
    }
};

class MeshGenerator
{
    const std::shared_ptr<CachedSDF> _cachedSDF;
    const IndexMap _index;
    const std::shared_ptr<std::optional<MeshData>[]> _meshDatas;

    MeshCpu BuildMesh(const int& totalVertices);
    inline std::optional<MeshData> Polygonize(float isolevel, int cube[8]);

  public:
    MeshCpu GenerateMesh();

    MeshGenerator(std::shared_ptr<CachedSDF> cachedSDF)
        : _cachedSDF(cachedSDF),
          _index(this->_cachedSDF->Index.Size - glm::uvec3(1)),
          _meshDatas(std::unique_ptr<std::optional<MeshData>[]>(new std::optional<MeshData>[_index.TotalSize]))
    {
    }
};
