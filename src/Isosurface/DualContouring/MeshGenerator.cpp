#include <Isosurface/DualContouring/MeshGenerator.h>
#include <algorithm>
#include <omp.h>

#define EDGE_NO_SIGN_CHANGE -1
#define EDGE_BORDER -2

MeshCpu MeshGenerator::BuildMesh(const int& totalVertices)
{
    MeshCpu mesh = {};
    mesh.VertexCount = totalVertices;

    mesh.Vertices = std::unique_ptr<glm::vec3[]>(new glm::vec3[mesh.VertexCount]);
#pragma omp parallel for schedule(auto)
    for (int i = 0; i < Index.TotalSize; i++)
    {
        PopulateMesh(i, mesh);
    }

    mesh.CalculateNormals();
    return mesh;
}

void MeshGenerator::PushEdge(MeshCpu& meshCpu, const int& offset, const float& dir, const glm::ivec3& shared0, const glm::ivec3& shared1, const glm::ivec3& unique0, const glm::ivec3& unique1)
{
    if (dir == 1)
    {
        meshCpu.Vertices[offset + 0] = _cubeVertices[Index.Encode(shared0)];
        meshCpu.Vertices[offset + 1] = _cubeVertices[Index.Encode(shared1)];
        meshCpu.Vertices[offset + 2] = _cubeVertices[Index.Encode(unique0)];

        meshCpu.Vertices[offset + 3] = _cubeVertices[Index.Encode(shared1)];
        meshCpu.Vertices[offset + 4] = _cubeVertices[Index.Encode(shared0)];
        meshCpu.Vertices[offset + 5] = _cubeVertices[Index.Encode(unique1)];
    }
    else
    {
        meshCpu.Vertices[offset + 5] = _cubeVertices[Index.Encode(shared0)];
        meshCpu.Vertices[offset + 4] = _cubeVertices[Index.Encode(shared1)];
        meshCpu.Vertices[offset + 3] = _cubeVertices[Index.Encode(unique0)];

        meshCpu.Vertices[offset + 2] = _cubeVertices[Index.Encode(shared1)];
        meshCpu.Vertices[offset + 1] = _cubeVertices[Index.Encode(shared0)];
        meshCpu.Vertices[offset + 0] = _cubeVertices[Index.Encode(unique1)];
    }
}

void MeshGenerator::PopulateMesh(const int& index, MeshCpu& meshCpu)
{
    glm::ivec3 coord = Index.Decode(index);

    int iX = _edgeMapX.Index.Encode(coord);
    int oX = _edgeMapX.IndexOffset[iX];
    if (oX >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::ivec3(coord.x, coord.y - 1, coord.z - 1);

        auto unique0 = glm::ivec3(coord.x, coord.y - 1, coord.z);
        auto unique1 = glm::ivec3(coord.x, coord.y, coord.z - 1);

        PushEdge(meshCpu, oX, _edgeMapX.Direction[iX], shared0, shared1, unique0, unique1);
    }

    int iY = _edgeMapY.Index.Encode(coord);
    int oY = _edgeMapY.IndexOffset[iY];
    if (oY >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::ivec3(coord.x - 1, coord.y, coord.z - 1);

        auto unique0 = glm::ivec3(coord.x, coord.y, coord.z - 1);
        auto unique1 = glm::ivec3(coord.x - 1, coord.y, coord.z);

        PushEdge(meshCpu, oY, _edgeMapY.Direction[iY], shared0, shared1, unique0, unique1);
    }

    int iZ = _edgeMapZ.Index.Encode(coord);
    int oZ = _edgeMapZ.IndexOffset[iZ];
    if (oZ >= 0)
    {
        auto shared0 = coord;
        auto shared1 = glm::ivec3(coord.x - 1, coord.y - 1, coord.z);

        auto unique0 = glm::ivec3(coord.x - 1, coord.y, coord.z);
        auto unique1 = glm::ivec3(coord.x, coord.y - 1, coord.z);

        PushEdge(meshCpu, oZ, _edgeMapZ.Direction[iZ], shared0, shared1, unique0, unique1);
    }
}

void MeshGenerator::CalculateEdge(int index)
{
    float d0 = _cachedSDF->Distances[index];
    glm::vec3 p0 = _cachedSDF->Positions[index];
    glm::ivec3 coord = _cachedSDF->Index.Decode(index);

    CalculateEdgeX(coord, d0, p0);
    CalculateEdgeY(coord, d0, p0);
    CalculateEdgeZ(coord, d0, p0);
}

void MeshGenerator::CalculateEdgeX(const glm::ivec3& coord, const float& d0, const glm::vec3& p0)
{
    if (coord.x >= _edgeMapX.Index.Size.x)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(glm::ivec3(coord.x + 1, coord.y, coord.z), d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapX.Index.Encode(coord);

    _edgeMapX.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapX.Direction[index] = NonZeroSign(d1);

    if (coord.y > 0 && coord.z > 0 && coord.y < Index.Size.y && coord.z < Index.Size.z)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapX.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y - 1, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y - 1, coord.z - 1))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y, coord.z - 1))] = true;
    }
    else
    {
        _edgeMapX.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeY(const glm::ivec3& coord, const float& d0, const glm::vec3& p0)
{
    if (coord.y >= _edgeMapY.Index.Size.y)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(glm::ivec3(coord.x, coord.y + 1, coord.z), d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapY.Index.Encode(coord);

    _edgeMapY.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapY.Direction[index] = NonZeroSign(d1);

    if (coord.x > 0 && coord.z > 0 && coord.x < Index.Size.x && coord.z < Index.Size.z)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapY.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x - 1, coord.y, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x - 1, coord.y, coord.z - 1))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y, coord.z - 1))] = true;
    }
    else
    {
        _edgeMapY.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeZ(const glm::ivec3& coord, const float& d0, const glm::vec3& p0)
{
    if (coord.z >= _edgeMapZ.Index.Size.z)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(glm::ivec3(coord.x, coord.y, coord.z + 1), d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapZ.Index.Encode(coord);

    _edgeMapZ.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapZ.Direction[index] = NonZeroSign(d1);

    if (coord.x > 0 && coord.y > 0 && coord.x < Index.Size.x && coord.y < Index.Size.y)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapZ.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x - 1, coord.y, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x - 1, coord.y - 1, coord.z))] = true;
        _cubeCheck[Index.Encode(glm::ivec3(coord.x, coord.y - 1, coord.z))] = true;
    }
    else
    {
        _edgeMapZ.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateVertex(int index)
{
    glm::ivec3 coord = Index.Decode(index);
    if (!_cubeCheck[index])
        return;

    int cnt = 0;
    glm::vec3 sum = glm::vec3(0.0f);

    AggregateEdge(_edgeMapX, glm::ivec3(coord.x, coord.y, coord.z), sum, cnt);
    AggregateEdge(_edgeMapX, glm::ivec3(coord.x, coord.y + 1, coord.z), sum, cnt);
    AggregateEdge(_edgeMapX, glm::ivec3(coord.x, coord.y + 1, coord.z + 1), sum, cnt);
    AggregateEdge(_edgeMapX, glm::ivec3(coord.x, coord.y, coord.z + 1), sum, cnt);

    AggregateEdge(_edgeMapY, glm::ivec3(coord.x, coord.y, coord.z), sum, cnt);
    AggregateEdge(_edgeMapY, glm::ivec3(coord.x + 1, coord.y, coord.z), sum, cnt);
    AggregateEdge(_edgeMapY, glm::ivec3(coord.x + 1, coord.y, coord.z + 1), sum, cnt);
    AggregateEdge(_edgeMapY, glm::ivec3(coord.x, coord.y, coord.z + 1), sum, cnt);

    AggregateEdge(_edgeMapZ, glm::ivec3(coord.x, coord.y, coord.z), sum, cnt);
    AggregateEdge(_edgeMapZ, glm::ivec3(coord.x + 1, coord.y, coord.z), sum, cnt);
    AggregateEdge(_edgeMapZ, glm::ivec3(coord.x + 1, coord.y + 1, coord.z), sum, cnt);
    AggregateEdge(_edgeMapZ, glm::ivec3(coord.x, coord.y + 1, coord.z), sum, cnt);

    _cubeVertices[index] = sum / (float)cnt;
}

void MeshGenerator::AggregateEdge(const EdgeMap& edgeMap, const glm::ivec3& coord, glm::vec3& sum, int& cnt)
{
    int index = edgeMap.Index.Encode(coord);
    if (edgeMap.IndexOffset[index] != EDGE_NO_SIGN_CHANGE)
    {
        sum += edgeMap.Intersection[index];
        cnt++;
    }
}

MeshCpu MeshGenerator::GenerateMesh()
{
    std::fill(_cubeCheck.get(), _cubeCheck.get() + Index.TotalSize, false);
    std::fill(_edgeMapX.IndexOffset.get(), _edgeMapX.IndexOffset.get() + _edgeMapX.Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapY.IndexOffset.get(), _edgeMapY.IndexOffset.get() + _edgeMapY.Index.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapZ.IndexOffset.get(), _edgeMapZ.IndexOffset.get() + _edgeMapZ.Index.TotalSize, EDGE_NO_SIGN_CHANGE);

    _counter = 0;
#pragma omp parallel
    {
#pragma omp for schedule(auto)
        for (int i = 0; i < _cachedSDF->Index.TotalSize; i++)
        {
            CalculateEdge(i);
        }
#pragma omp for schedule(auto)
        for (int i = 0; i < Index.TotalSize; i++)
        {
            CalculateVertex(i);
        }
    }

    return BuildMesh(_counter * 6);
}
