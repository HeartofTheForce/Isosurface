#pragma once
#include <memory>
#include <glad/gl.h>
#include <glm/glm.hpp>

struct MeshGpu
{
    GLuint VaoId{0};
    int VertexCount{0};

    int VboCount{0};
    std::unique_ptr<GLuint[]> VboIds{nullptr};
    std::unique_ptr<GLuint[]> VboStrides{nullptr};

    void LoadBuffer(const int &vboIndex, const void *data, const int &stride);

    MeshGpu() = default;
    ~MeshGpu();
    MeshGpu(const MeshGpu &) = delete;
    MeshGpu &operator=(const MeshGpu &) = delete;
    MeshGpu(MeshGpu &&other)
    {
        *this = std::move(other);
    };
    MeshGpu &operator=(MeshGpu &&other)
    {
        VaoId = other.VaoId;
        other.VaoId = 0;

        VboCount = other.VboCount;
        other.VboCount = 0;

        VboIds = std::move(other.VboIds);
        VboStrides = std::move(other.VboStrides);

        return *this;
    };
};
