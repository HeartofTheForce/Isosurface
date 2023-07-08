#include <Isosurface/CachedSDF.h>

void CachedSDF::Measure(const glm::mat4& localToWorld, const SDF& f)
{
    for (int z = 0; z < Index.SizeZ; z++)
    {
        for (int y = 0; y < Index.SizeY; y++)
        {
            for (int x = 0; x < Index.SizeX; x++)
            {
                int index = Index.Encode(x, y, z);
                glm::vec3 p = localToWorld * glm::vec4(x, y, z, 1.0f);

                if (x == 0 || x == Index.SizeX - 1 ||
                    y == 0 || y == Index.SizeY - 1 ||
                    z == 0 || z == Index.SizeZ - 1)
                    CachedDistances[index] = 0;
                else
                    CachedDistances[index] = f(p);
                CachedPositions[index] = p;
            }
        }
    }
}
