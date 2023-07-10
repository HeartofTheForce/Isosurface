#include <Isosurface/CachedSDF.h>

void CachedSDF::Measure(const glm::mat4& localToWorld, const SDF::SDF& f)
{
    for (glm::uint z = 0; z < Index.Size.z; z++)
    {
        for (glm::uint y = 0; y < Index.Size.y; y++)
        {
            for (glm::uint x = 0; x < Index.Size.x; x++)
            {
                int index = Index.Encode(glm::uvec3(x, y, z));
                glm::vec3 p = localToWorld * glm::vec4(x, y, z, 1.0f);

                if (x == 0 || x == Index.Size.x - 1 ||
                    y == 0 || y == Index.Size.y - 1 ||
                    z == 0 || z == Index.Size.z - 1)
                    Distances[index] = 0;
                else
                    Distances[index] = f(p);
                Positions[index] = p;
            }
        }
    }
}
