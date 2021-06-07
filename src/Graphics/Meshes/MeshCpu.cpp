#include <Graphics/Meshes/MeshCpu.h>

namespace
{
    template <typename T>
    void LoadBuffer(MeshGpu &meshGpu, int vboIndex, const T *data)
    {
        meshGpu.LoadBuffer(vboIndex, data, sizeof(T));
    }
} // namespace

void MeshCpu::CalculateNormals()
{
    assert(VertexCount % 3 == 0);

    Normals = std::unique_ptr<glm::vec3[]>(new glm::vec3[VertexCount]);
#pragma omp parallel for schedule(auto)
    for (int i = 0; i < VertexCount; i += 3)
    {
        glm::vec3 u = Vertices[i + 1] - Vertices[i + 0];
        glm::vec3 v = Vertices[i + 2] - Vertices[i + 0];

        glm::vec3 normal = glm::normalize(glm::cross(v, u));

        Normals[i + 0] = normal;
        Normals[i + 1] = normal;
        Normals[i + 2] = normal;
    }
}

const int MeshCpu::VboCount = 4;
const int MeshCpu::VertexVboIndex = 0;
const int MeshCpu::NormalVboIndex = 1;
const int MeshCpu::TexCoordVboIndex = 2;
const int MeshCpu::ColorVboIndex = 3;

MeshGpu MeshCpu::LoadGpu(Program &program)
{
    assert(Vertices != nullptr);

    MeshGpu meshGpu = {};

    meshGpu.VboCount = VboCount;
    meshGpu.VertexCount = VertexCount;
    meshGpu.VboIds = std::unique_ptr<GLuint[]>(new GLuint[meshGpu.VboCount]{0});
    meshGpu.VboStrides = std::unique_ptr<GLuint[]>(new GLuint[meshGpu.VboCount]{0});

    LoadBuffer(meshGpu, VertexVboIndex, Vertices.get());
    LoadBuffer(meshGpu, NormalVboIndex, Normals.get());
    LoadBuffer(meshGpu, TexCoordVboIndex, TexCoords.get());
    LoadBuffer(meshGpu, ColorVboIndex, Colors.get());

    program.GenerateVao(meshGpu);

    return meshGpu;
}
