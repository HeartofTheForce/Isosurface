#pragma once
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <DualContouring/Coordinate.h>

typedef std::function<float(glm::vec3)> SDF;

struct CachedSDF
{
    const int SizeX;
    const int SizeY;
    const int SizeZ;
    const int TotalSize;

    const std::unique_ptr<float[]> CachedDistances;
    const std::unique_ptr<glm::vec3[]> CachedPositions;

    void Measure(const glm::mat4 &localToWorld, const SDF &f);
    inline void Get(const int &x, const int &y, const int &z, float &distance, glm::vec3 &position) const
    {
        int index = Coordinate::To1D(x, y, z, SizeX, SizeY);

        distance = CachedDistances[index];
        position = CachedPositions[index];
    }

    CachedSDF(
        const int &sizeX,
        const int &sizeY,
        const int &sizeZ)
        : SizeX(sizeX),
          SizeY(sizeY),
          SizeZ(sizeZ),
          TotalSize(SizeX * SizeY * SizeZ),
          CachedDistances(std::unique_ptr<float[]>(new float[TotalSize])),
          CachedPositions(std::unique_ptr<glm::vec3[]>(new glm::vec3[TotalSize])) {}

    CachedSDF(const CachedSDF &) = delete;
    CachedSDF &operator=(const CachedSDF &) = delete;
};
