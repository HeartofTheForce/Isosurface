#pragma once
#include <Isosurface/DualContouring/IndexMap.h>
#include <functional>
#include <glm/glm.hpp>
#include <memory>

typedef std::function<float(glm::vec3)> SDF;

struct CachedSDF
{
    const IndexMap Index;

    const std::unique_ptr<float[]> CachedDistances;
    const std::unique_ptr<glm::vec3[]> CachedPositions;

    void Measure(const glm::mat4& localToWorld, const SDF& f);
    inline void Get(const glm::ivec3& coord, float& distance, glm::vec3& position) const
    {
        int index = Index.Encode(coord);

        distance = CachedDistances[index];
        position = CachedPositions[index];
    }

    CachedSDF(
        const glm::ivec3& size)
        : Index(size),
          CachedDistances(std::unique_ptr<float[]>(new float[Index.TotalSize])),
          CachedPositions(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize]))
    {
    }
};
