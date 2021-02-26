#pragma once
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <DualContouring/IndexMap.h>

typedef std::function<float(glm::vec3)> SDF;

struct CachedSDF
{
    const IndexMap Index;

    const std::unique_ptr<float[]> CachedDistances;
    const std::unique_ptr<glm::vec3[]> CachedPositions;

    void Measure(const glm::mat4 &localToWorld, const SDF &f);
    inline void Get(const int &x, const int &y, const int &z, float &distance, glm::vec3 &position) const
    {
        int index = Index.To1D(x, y, z);

        distance = CachedDistances[index];
        position = CachedPositions[index];
    }

    CachedSDF(
        const int &sizeX,
        const int &sizeY,
        const int &sizeZ)
        : Index(sizeX, sizeY, sizeZ),
          CachedDistances(std::unique_ptr<float[]>(new float[Index.TotalSize])),
          CachedPositions(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize])) {}
};
