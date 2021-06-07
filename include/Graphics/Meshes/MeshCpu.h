#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <Graphics/Meshes/MeshGpu.h>
#include <Graphics/Programs/Program.h>

struct MeshCpu
{
    const static int VboCount;
    const static int VertexVboIndex;
    const static int NormalVboIndex;
    const static int TexCoordVboIndex;
    const static int ColorVboIndex;

    int VertexCount{0};
    std::unique_ptr<glm::vec3[]> Vertices{nullptr};
    std::unique_ptr<glm::vec3[]> Normals{nullptr};
    std::unique_ptr<glm::vec2[]> TexCoords{nullptr};
    std::unique_ptr<glm::vec3[]> Colors{nullptr};

    void CalculateNormals();
    MeshGpu LoadGpu(Program &program);
};
