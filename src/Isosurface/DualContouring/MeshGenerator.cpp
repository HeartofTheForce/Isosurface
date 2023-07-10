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

    int iX = _edgeMapX.Index.Encode(coord);
    int oX = _edgeMapX.IndexOffset[iX];
    if (oX >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::uvec3(coord.x, coord.y - 1, coord.z - 1);

        auto unique0 = glm::uvec3(coord.x, coord.y - 1, coord.z);
        auto unique1 = glm::uvec3(coord.x, coord.y, coord.z - 1);

        PushEdge(meshCpu, oX, _edgeMapX.Directions[iX], shared0, shared1, unique0, unique1);
    }

    int iY = _edgeMapY.Index.Encode(coord);
    int oY = _edgeMapY.IndexOffset[iY];
    if (oY >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::uvec3(coord.x - 1, coord.y, coord.z - 1);

        auto unique0 = glm::uvec3(coord.x, coord.y, coord.z - 1);
        auto unique1 = glm::uvec3(coord.x - 1, coord.y, coord.z);

        PushEdge(meshCpu, oY, _edgeMapY.Directions[iY], shared0, shared1, unique0, unique1);
    }

    int iZ = _edgeMapZ.Index.Encode(coord);
    int oZ = _edgeMapZ.IndexOffset[iZ];
    if (oZ >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::uvec3(coord.x - 1, coord.y - 1, coord.z);

        auto unique0 = glm::uvec3(coord.x - 1, coord.y, coord.z);
        auto unique1 = glm::uvec3(coord.x, coord.y - 1, coord.z);

        PushEdge(meshCpu, oZ, _edgeMapZ.Directions[iZ], shared0, shared1, unique0, unique1);
    }
}

void MeshGenerator::CalculateEdge(int index)
{
    float d0 = _cachedSDF->Distances[index];
    glm::vec3 p0 = _cachedSDF->Positions[index];
    glm::vec3 n0 = _cachedSDF->Normals[index];
    glm::uvec3 coord = _cachedSDF->Index.Decode(index);

    CalculateEdgeX(coord, d0, p0, n0);
    CalculateEdgeY(coord, d0, p0, n0);
    CalculateEdgeZ(coord, d0, p0, n0);
}

void MeshGenerator::CalculateEdgeX(const glm::uvec3& coord, const float& d0, const glm::vec3& p0, const glm::vec3& n0)
{
    if (coord.x >= _edgeMapX.Index.Size.x)
        return;

    float d1;
    glm::vec3 p1;
    glm::vec3 n1;
    _cachedSDF->Get(glm::uvec3(coord.x + 1, coord.y, coord.z), d1, p1, n1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapX.Index.Encode(coord);

    _edgeMapX.Intersections[index] = Interpolate(0, p0, p1, d0, d1);
    _edgeMapX.Normals[index] = Interpolate(0, n0, n1, d0, d1);
    _edgeMapX.Directions[index] = NonZeroSign(d1);

    if (coord.y > 0 && coord.z > 0 && coord.y < _index.Size.y && coord.z < _index.Size.z)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapX.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y - 1, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y - 1, coord.z - 1))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y, coord.z - 1))] = true;
    }
    else
    {
        _edgeMapX.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeY(const glm::uvec3& coord, const float& d0, const glm::vec3& p0, const glm::vec3& n0)
{
    if (coord.y >= _edgeMapY.Index.Size.y)
        return;

    float d1;
    glm::vec3 p1;
    glm::vec3 n1;
    _cachedSDF->Get(glm::uvec3(coord.x, coord.y + 1, coord.z), d1, p1, n1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapY.Index.Encode(coord);

    _edgeMapY.Intersections[index] = Interpolate(0, p0, p1, d0, d1);
    _edgeMapY.Normals[index] = Interpolate(0, n0, n1, d0, d1);
    _edgeMapY.Directions[index] = NonZeroSign(d1);

    if (coord.x > 0 && coord.z > 0 && coord.x < _index.Size.x && coord.z < _index.Size.z)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapY.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x - 1, coord.y, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x - 1, coord.y, coord.z - 1))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y, coord.z - 1))] = true;
    }
    else
    {
        _edgeMapY.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeZ(const glm::uvec3& coord, const float& d0, const glm::vec3& p0, const glm::vec3& n0)
{
    if (coord.z >= _edgeMapZ.Index.Size.z)
        return;

    float d1;
    glm::vec3 p1;
    glm::vec3 n1;
    _cachedSDF->Get(glm::uvec3(coord.x, coord.y, coord.z + 1), d1, p1, n1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapZ.Index.Encode(coord);

    _edgeMapZ.Intersections[index] = Interpolate(0, p0, p1, d0, d1);
    _edgeMapZ.Normals[index] = Interpolate(0, n0, n1, d0, d1);
    _edgeMapZ.Directions[index] = NonZeroSign(d1);

    if (coord.x > 0 && coord.y > 0 && coord.x < _index.Size.x && coord.y < _index.Size.y)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapZ.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x - 1, coord.y, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x - 1, coord.y - 1, coord.z))] = true;
        _cubeCheck[_index.Encode(glm::uvec3(coord.x, coord.y - 1, coord.z))] = true;
    }
    else
    {
        _edgeMapZ.IndexOffset[index] = EDGE_BORDER;
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

    AggregateEdge(_edgeMapX, glm::uvec3(coord.x, coord.y, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapX, glm::uvec3(coord.x, coord.y + 1, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapX, glm::uvec3(coord.x, coord.y + 1, coord.z + 1), center, points, normals, cnt);
    AggregateEdge(_edgeMapX, glm::uvec3(coord.x, coord.y, coord.z + 1), center, points, normals, cnt);

    AggregateEdge(_edgeMapY, glm::uvec3(coord.x, coord.y, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapY, glm::uvec3(coord.x + 1, coord.y, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapY, glm::uvec3(coord.x + 1, coord.y, coord.z + 1), center, points, normals, cnt);
    AggregateEdge(_edgeMapY, glm::uvec3(coord.x, coord.y, coord.z + 1), center, points, normals, cnt);

    AggregateEdge(_edgeMapZ, glm::uvec3(coord.x, coord.y, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapZ, glm::uvec3(coord.x + 1, coord.y, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapZ, glm::uvec3(coord.x + 1, coord.y + 1, coord.z), center, points, normals, cnt);
    AggregateEdge(_edgeMapZ, glm::uvec3(coord.x, coord.y + 1, coord.z), center, points, normals, cnt);

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
    std::fill(_edgeMapX.IndexOffset.get(), _edgeMapX.IndexOffset.get() + _edgeMapX.Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapY.IndexOffset.get(), _edgeMapY.IndexOffset.get() + _edgeMapY.Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapZ.IndexOffset.get(), _edgeMapZ.IndexOffset.get() + _edgeMapZ.Index.TotalSize, EDGE_NO_SIGN_CHANGE);

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