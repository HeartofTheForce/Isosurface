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
          Index(this->_cachedSDF->Index.Size - glm::ivec3(1))
    {
    }
};

class MeshData
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    MeshData()
    {
        this->vertices = std::vector<glm::vec3>();
        this->normals = std::vector<glm::vec3>();
    }

    MeshData(int capacity)
    {
        this->vertices = std::vector<glm::vec3>();
        this->normals = std::vector<glm::vec3>();

        this->vertices.reserve(capacity);
        this->normals.reserve(capacity);
    }
};
