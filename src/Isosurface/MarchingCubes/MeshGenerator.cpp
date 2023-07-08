#include <Graphics/Meshes/MeshCpu.h>
#include <Isosurface/CachedSDF.h>
#include <Isosurface/MarchingCubes/Lookup.h>
#include <Isosurface/MarchingCubes/MeshGenerator.h>

MeshCpu MeshGenerator::GenerateMesh()
{
    auto meshDatas = std::unique_ptr<std::optional<MeshData>[]>(new std::optional<MeshData>[Index.TotalSize]);

#pragma omp parallel
    {
#pragma omp for schedule(auto)
        for (int x = 0; x < Index.Size.x; x++)
        {
            for (int y = 0; y < Index.Size.y; y++)
            {
                for (int z = 0; z < Index.Size.z; z++)
                {
                    auto coord = glm::ivec3(x, y, z);

                    int cube[8] = {
                        _cachedSDF->Index.Encode(coord + glm::ivec3(0, 0, 0)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(1, 0, 0)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(1, 0, 1)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(0, 0, 1)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(0, 1, 0)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(1, 1, 0)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(1, 1, 1)),
                        _cachedSDF->Index.Encode(coord + glm::ivec3(0, 1, 1)),
                    };
                    auto meshData = Polygonize(
                        0.0,
                        cube
                    );
                    auto index = Index.Encode(coord);

                    meshDatas[index] = meshData;
                }
            }
        }
    }

    MeshData root = {};
    for (int i = 0; i < Index.TotalSize; i++)
    {
        if (meshDatas[i].has_value())
            MeshData::Merge(root, meshDatas[i].value());
    }

    MeshCpu mesh = {};
    mesh.VertexCount = root.vertices.size();

    mesh.Vertices = std::unique_ptr<glm::vec3[]>(new glm::vec3[mesh.VertexCount]);
    std::copy(root.vertices.begin(), root.vertices.end(), mesh.Vertices.get());

    mesh.CalculateNormals();

    return mesh;
}

std::optional<MeshData> MeshGenerator::Polygonize(float isolevel, int cube[8])
{
    float distances[8] = {
        _cachedSDF->Distances[cube[0]],
        _cachedSDF->Distances[cube[1]],
        _cachedSDF->Distances[cube[2]],
        _cachedSDF->Distances[cube[3]],
        _cachedSDF->Distances[cube[4]],
        _cachedSDF->Distances[cube[5]],
        _cachedSDF->Distances[cube[6]],
        _cachedSDF->Distances[cube[7]],
    };
    glm::vec3 positions[8] = {
        _cachedSDF->Positions[cube[0]],
        _cachedSDF->Positions[cube[1]],
        _cachedSDF->Positions[cube[2]],
        _cachedSDF->Positions[cube[3]],
        _cachedSDF->Positions[cube[4]],
        _cachedSDF->Positions[cube[5]],
        _cachedSDF->Positions[cube[6]],
        _cachedSDF->Positions[cube[7]],
    };

    int cubeindex = 0;
    if (distances[0] < isolevel)
        cubeindex |= 1;
    if (distances[1] < isolevel)
        cubeindex |= 2;
    if (distances[2] < isolevel)
        cubeindex |= 4;
    if (distances[3] < isolevel)
        cubeindex |= 8;
    if (distances[4] < isolevel)
        cubeindex |= 16;
    if (distances[5] < isolevel)
        cubeindex |= 32;
    if (distances[6] < isolevel)
        cubeindex |= 64;
    if (distances[7] < isolevel)
        cubeindex |= 128;

    if (edgeTable[cubeindex] == 0x0)
        return std::nullopt;

    glm::vec3 vertlist[12];

    if (edgeTable[cubeindex] & 1)
        vertlist[0] =
            VertexInterp(isolevel, positions[0], positions[1], distances[0], distances[1]);
    if (edgeTable[cubeindex] & 2)
        vertlist[1] =
            VertexInterp(isolevel, positions[1], positions[2], distances[1], distances[2]);
    if (edgeTable[cubeindex] & 4)
        vertlist[2] =
            VertexInterp(isolevel, positions[2], positions[3], distances[2], distances[3]);
    if (edgeTable[cubeindex] & 8)
        vertlist[3] =
            VertexInterp(isolevel, positions[3], positions[0], distances[3], distances[0]);
    if (edgeTable[cubeindex] & 16)
        vertlist[4] =
            VertexInterp(isolevel, positions[4], positions[5], distances[4], distances[5]);
    if (edgeTable[cubeindex] & 32)
        vertlist[5] =
            VertexInterp(isolevel, positions[5], positions[6], distances[5], distances[6]);
    if (edgeTable[cubeindex] & 64)
        vertlist[6] =
            VertexInterp(isolevel, positions[6], positions[7], distances[6], distances[7]);
    if (edgeTable[cubeindex] & 128)
        vertlist[7] =
            VertexInterp(isolevel, positions[7], positions[4], distances[7], distances[4]);
    if (edgeTable[cubeindex] & 256)
        vertlist[8] =
            VertexInterp(isolevel, positions[0], positions[4], distances[0], distances[4]);
    if (edgeTable[cubeindex] & 512)
        vertlist[9] =
            VertexInterp(isolevel, positions[1], positions[5], distances[1], distances[5]);
    if (edgeTable[cubeindex] & 1024)
        vertlist[10] =
            VertexInterp(isolevel, positions[2], positions[6], distances[2], distances[6]);
    if (edgeTable[cubeindex] & 2048)
        vertlist[11] =
            VertexInterp(isolevel, positions[3], positions[7], distances[3], distances[7]);

    MeshData meshData = {16};

    int ntriang = 0;
    for (int i = 0; triTable[cubeindex][i] != -1; i += 3)
    {
        meshData.vertices.push_back(vertlist[triTable[cubeindex][i + 2]]);
        meshData.vertices.push_back(vertlist[triTable[cubeindex][i + 1]]);
        meshData.vertices.push_back(vertlist[triTable[cubeindex][i]]);
        ntriang++;
    }

    return std::optional(meshData);
}
