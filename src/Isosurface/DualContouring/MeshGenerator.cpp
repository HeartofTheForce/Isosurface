#include <Isosurface/DualContouring/MeshGenerator.h>
#include <algorithm>
#include <omp.h>

#define EDGE_NO_SIGN_CHANGE -1
#define EDGE_BORDER -2
#define SHARP_FEATURES

namespace DualContouring
{
MeshCpu MeshGenerator::BuildMesh(const int& totalVertices)
{
    MeshCpu mesh = {};
    mesh.VertexCount = totalVertices;

    mesh.Vertices = std::unique_ptr<glm::vec3[]>(new glm::vec3[mesh.VertexCount]);
#pragma omp parallel for schedule(auto)
    for (glm::uint i = 0; i < _index.TotalSize; i++)
    {
        PopulateMesh(i, mesh);
    }

    mesh.CalculateNormals();
    return mesh;
}

void MeshGenerator::PushEdge(MeshCpu& meshCpu, const int& offset, const float& dir, const glm::uvec3& shared0, const glm::uvec3& shared1, const glm::uvec3& unique0, const glm::uvec3& unique1)
{
    if (dir == 1)
    {
        meshCpu.Vertices[offset + 0] = _cubeVertices[_index.Encode(shared0)];
        meshCpu.Vertices[offset + 1] = _cubeVertices[_index.Encode(shared1)];
        meshCpu.Vertices[offset + 2] = _cubeVertices[_index.Encode(unique0)];

        meshCpu.Vertices[offset + 3] = _cubeVertices[_index.Encode(shared1)];
        meshCpu.Vertices[offset + 4] = _cubeVertices[_index.Encode(shared0)];
        meshCpu.Vertices[offset + 5] = _cubeVertices[_index.Encode(unique1)];
    }
    else
    {
        meshCpu.Vertices[offset + 5] = _cubeVertices[_index.Encode(shared0)];
        meshCpu.Vertices[offset + 4] = _cubeVertices[_index.Encode(shared1)];
        meshCpu.Vertices[offset + 3] = _cubeVertices[_index.Encode(unique0)];

        meshCpu.Vertices[offset + 2] = _cubeVertices[_index.Encode(shared1)];
        meshCpu.Vertices[offset + 1] = _cubeVertices[_index.Encode(shared0)];
        meshCpu.Vertices[offset + 0] = _cubeVertices[_index.Encode(unique1)];
    }
}

void MeshGenerator::PopulateMesh(const int& index, MeshCpu& meshCpu)
{
    glm::uvec3 coord = _index.Decode(index);

    for (int iA = 0; iA < 3; iA++)
    {
        int iB = (iA + 1) % 3;
        int iC = (iA + 2) % 3;

        auto edgeMap = &_edgeMaps[iA];
        int edgeIndex = edgeMap->Index.Encode(coord);
        int offset = edgeMap->IndexOffset[edgeIndex];
        if (offset >= 0)
        {
            auto shared0 = coord;
            auto shared1 = coord;

            shared1[iB] -= 1;
            shared1[iC] -= 1;

            auto unique0 = coord;
            auto unique1 = coord;

            unique0[iB] -= 1;
            unique1[iC] -= 1;

            PushEdge(meshCpu, offset, edgeMap->Directions[edgeIndex], shared0, shared1, unique0, unique1);
        }
    }
}

void MeshGenerator::CalculateEdge(int index)
{
    float d0 = _cachedSDF->Distances[index];
    glm::vec3 p0 = _cachedSDF->Positions[index];
    glm::vec3 n0 = _cachedSDF->Normals[index];
    glm::uvec3 coord = _cachedSDF->Index.Decode(index);

    CalculateEdge(0, coord, d0, p0, n0);
    CalculateEdge(1, coord, d0, p0, n0);
    CalculateEdge(2, coord, d0, p0, n0);
}

void MeshGenerator::CalculateEdge(const int iA, const glm::uvec3& coord, const float& d0, const glm::vec3& p0, const glm::vec3& n0)
{
    auto edgeMap = &_edgeMaps[iA];
    if (coord[iA] >= edgeMap->Index.Size[iA])
        return;

    float d1;
    glm::vec3 p1;
    glm::vec3 n1;

    glm::uvec3 nextCoord = coord;
    nextCoord[iA] += 1;
    _cachedSDF->Get(nextCoord, d1, p1, n1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = edgeMap->Index.Encode(coord);

    edgeMap->Intersections[index] = Interpolate(0, p0, p1, d0, d1);
    edgeMap->Normals[index] = Interpolate(0, n0, n1, d0, d1);
    edgeMap->Directions[index] = NonZeroSign(d1);

    int iB = (iA + 1) % 3;
    int iC = (iA + 2) % 3;

    if (coord[iB] > 0 && coord[iC] > 0 && coord[iB] < _index.Size[iB] && coord[iC] < _index.Size[iC])
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        edgeMap->IndexOffset[index] = indexOffset * 6;

        glm::uvec3 coords[] = {coord, coord, coord, coord};

        coords[1][iB] -= 1;
        coords[2][iB] -= 1;
        coords[2][iC] -= 1;
        coords[3][iC] -= 1;

        for (int i = 0; i < 4; i++)
        {
            _cubeCheck[_index.Encode(coords[i])] = true;
        }
    }
    else
    {
        edgeMap->IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateVertex(int index)
{
    glm::uvec3 coord = _index.Decode(index);
    if (!_cubeCheck[index])
        return;

    glm::vec3 center = glm::vec3(0.0f);
    glm::vec3 points[12];
    glm::vec3 normals[12];
    size_t cnt = 0;

    for (int iA = 0; iA < 3; iA++)
    {
        int iB = (iA + 1) % 3;
        int iC = (iA + 2) % 3;

        glm::uvec3 coords[] = {coord, coord, coord, coord};
        coords[1][iB] += 1;
        coords[2][iB] += 1;
        coords[2][iC] += 1;
        coords[3][iC] += 1;

        for (int i = 0; i < 4; i++)
        {
            AggregateEdge(_edgeMaps[iA], coords[i], center, points, normals, cnt);
        }
    }

    if (cnt == 0)
        return;

    center = center / (float)cnt;

#ifdef SHARP_FEATURES
    // particle?
    const size_t MAX_ITERATIONS = 100;
    const float THRESHOLD = 0.001f * 0.001f;
    for (size_t i = 0; i < MAX_ITERATIONS; i++)
    {
        glm::vec3 force = glm::vec3();

        for (size_t j = 0; j < cnt; j++)
        {
            auto point = points[j];
            auto normal = normals[j];

            // distance to plane
            force += normal * -1.0f * (glm::dot(normal, center - point));
        }

        // dampen the force
        float damping = 1.0f - (float)i / MAX_ITERATIONS;

        center += force * damping / (float)cnt; // average over all the points

        // if the force is negligible according to the threshold, we're done
        if (glm::dot(force, force) < THRESHOLD)
            break;
    }

    // clamp
    float dMin;
    glm::vec3 pMin;
    glm::vec3 nMin;
    _cachedSDF->Get(coord, dMin, pMin, nMin);

    float dMax;
    glm::vec3 pMax;
    glm::vec3 nMax;
    _cachedSDF->Get(coord + glm::uvec3(1), dMax, pMax, nMax);

    center = glm::clamp(center, pMin, pMax);
#endif

    _cubeVertices[index] = center;
}

void MeshGenerator::AggregateEdge(
    const EdgeMap& edgeMap,
    const glm::uvec3& coord,
    glm::vec3& sum,
    glm::vec3* intersections,
    glm::vec3* normals,
    size_t& cnt
)
{
    int index = edgeMap.Index.Encode(coord);
    if (edgeMap.IndexOffset[index] != EDGE_NO_SIGN_CHANGE)
    {
        auto intersection = edgeMap.Intersections[index];
        auto normal = edgeMap.Normals[index];
        sum += intersection;

        auto i = cnt++;
        intersections[i] = intersection;
        normals[i] = normal;
    }
}

MeshCpu MeshGenerator::GenerateMesh()
{
    std::fill(_cubeCheck.get(), _cubeCheck.get() + _index.TotalSize, false);
    std::fill(_edgeMaps[0].IndexOffset.get(), _edgeMaps[0].IndexOffset.get() + _edgeMaps[0].Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMaps[1].IndexOffset.get(), _edgeMaps[1].IndexOffset.get() + _edgeMaps[1].Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMaps[2].IndexOffset.get(), _edgeMaps[2].IndexOffset.get() + _edgeMaps[2].Index.TotalSize, EDGE_NO_SIGN_CHANGE);

    _counter = 0;
#pragma omp parallel
    {
#pragma omp for schedule(auto)
        for (glm::uint i = 0; i < _cachedSDF->Index.TotalSize; i++)
        {
            CalculateEdge(i);
        }
#pragma omp for schedule(auto)
        for (glm::uint i = 0; i < _index.TotalSize; i++)
        {
            CalculateVertex(i);
        }
    }

    return BuildMesh(_counter * 6);
}
} // namespace DualContouring