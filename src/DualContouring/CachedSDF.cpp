#include <DualContouring/CachedSDF.h>
#include <DualContouring/Coordinate.h>

void CachedSDF::Measure(const glm::mat4 &localToWorld, const SDF &f)
{
    for (int z = 0; z < SizeZ; z++)
    {
        for (int y = 0; y < SizeY; y++)
        {
            for (int x = 0; x < SizeX; x++)
            {
                int index = Coordinate::To1D(x, y, z, SizeX, SizeY);
                glm::vec3 p = localToWorld * glm::vec4(x, y, z, 1.0f);

                if (x == 0 || x == SizeX - 1 ||
                    y == 0 || y == SizeY - 1 ||
                    z == 0 || z == SizeZ - 1)
                    CachedDistances[index] = 0;
                else
                    CachedDistances[index] = f(p);
                CachedPositions[index] = p;
            }
        }
    }
}

void CachedSDF::Get(const int &x, const int &y, const int &z, float &distance, glm::vec3 &position) const
{
    int index = Coordinate::To1D(x, y, z, SizeX, SizeY);

    distance = CachedDistances[index];
    position = CachedPositions[index];
}
