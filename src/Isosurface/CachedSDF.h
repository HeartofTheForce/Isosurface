#pragma once
#include <Isosurface/IndexMap.h>
#include <Isosurface/SDF.h>
#include <glm/glm.hpp>
#include <memory>
 
struct CachedSDF
{
    const IndexMap Index;

    const std::unique_ptr<float[]> Distances;
    const std::unique_ptr<glm::vec3[]> Positions;

    void Measure(const glm::mat4& localToWorld, const SDF::SDF& f);
    inline void Get(const glm::uvec3& coord, float& distance, glm::vec3& position) const
    {
        int index = Index.Encode(coord);

        distance = Distances[index];
        position = Positions[index];
    }

    CachedSDF(
        const glm::uvec3& size)
        : Index(size),
          Distances(std::unique_ptr<float[]>(new float[Index.TotalSize])),
          Positions(std::unique_ptr<glm::vec3[]>(new glm::vec3[Index.TotalSize]))
    {
    }
};
