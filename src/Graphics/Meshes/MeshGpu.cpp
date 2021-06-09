#include <Graphics/Meshes/MeshGpu.h>

void MeshGpu::LoadBuffer(const int &vboIndex, const void *data, const int &stride)
{
    assert(VboIds[vboIndex] == 0);
    assert(VboStrides[vboIndex] == 0);

    if (data != nullptr)
    {
        glGenBuffers(1, &VboIds[vboIndex]);
        glBindBuffer(GL_ARRAY_BUFFER, VboIds[vboIndex]);
        glBufferData(GL_ARRAY_BUFFER, stride * VertexCount, data, GL_STATIC_DRAW);
        VboStrides[vboIndex] = stride;
    }
}

MeshGpu::~MeshGpu()
{
    glDeleteVertexArrays(1, &VaoId);
    glDeleteBuffers(VboCount, VboIds.get());
}

MeshGpu::MeshGpu(MeshGpu &&other)
{
    *this = std::move(other);
}

MeshGpu &MeshGpu::operator=(MeshGpu &&other)
{
    VaoId = other.VaoId;
    other.VaoId = 0;

    VboCount = other.VboCount;
    other.VboCount = 0;

    VboIds = std::move(other.VboIds);
    VboStrides = std::move(other.VboStrides);

    return *this;
}
