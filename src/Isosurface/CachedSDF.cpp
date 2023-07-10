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
                glm::vec3 position = localToWorld * glm::vec4(x, y, z, 1.0f);

                float distance;
                glm::vec3 normal;

                if (x == 0)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(-1, 0, 0));
                }
                else if (y == 0)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(0, -1, 0));
                }
                else if (z == 0)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(0, 0, -1));
                }
                else if (x == Index.Size.x - 1)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(1, 0, 0));
                }
                else if (y == Index.Size.y - 1)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(0, 1, 0));
                }
                else if (z == Index.Size.z - 1)
                {
                    distance = 0.0;
                    normal = glm::mat3(localToWorld) * (glm::vec3(0, 0, 1));
                }
                else
                {
                    distance = f(position);
                    normal = SDF::Normal(f, position, glm::vec3(0.001));
                }

                Positions[index] = position;
                Distances[index] = distance;
                Normals[index] = normal;
            }
        }
    }
}
