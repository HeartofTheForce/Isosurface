#include <Isosurface/Common.h>
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

void MeshGenerator::PopulateMesh(const int& index, MeshCpu& meshCpu)
{
    int x, y, z;
    Index.Decode(index, x, y, z);

    int iX = _edgeMapX.Index.Encode(x, y, z);
    int oX = _edgeMapX.IndexOffset[iX];
    if (oX >= 0)
    {
        if (_edgeMapX.Direction[iX] == 1)
        {
            meshCpu.Vertices[oX + 0] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oX + 1] = _cubeVertices[Index.Encode(x, y - 1, z - 1)];
            meshCpu.Vertices[oX + 2] = _cubeVertices[Index.Encode(x, y - 1, z)]; // unique

            meshCpu.Vertices[oX + 3] = _cubeVertices[Index.Encode(x, y - 1, z - 1)];
            meshCpu.Vertices[oX + 4] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oX + 5] = _cubeVertices[Index.Encode(x, y, z - 1)]; // unique
        }
        else
        {
            meshCpu.Vertices[oX + 5] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oX + 4] = _cubeVertices[Index.Encode(x, y - 1, z - 1)];
            meshCpu.Vertices[oX + 3] = _cubeVertices[Index.Encode(x, y - 1, z)]; // unique

            meshCpu.Vertices[oX + 2] = _cubeVertices[Index.Encode(x, y - 1, z - 1)];
            meshCpu.Vertices[oX + 1] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oX + 0] = _cubeVertices[Index.Encode(x, y, z - 1)]; // unique
        }
    }

    int iY = _edgeMapY.Index.Encode(x, y, z);
    int oY = _edgeMapY.IndexOffset[iY];
    if (oY >= 0)
    {
        if (_edgeMapY.Direction[iY] == 1)
        {
            meshCpu.Vertices[oY + 0] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oY + 1] = _cubeVertices[Index.Encode(x - 1, y, z - 1)];
            meshCpu.Vertices[oY + 2] = _cubeVertices[Index.Encode(x, y, z - 1)]; // unique

            meshCpu.Vertices[oY + 3] = _cubeVertices[Index.Encode(x - 1, y, z - 1)];
            meshCpu.Vertices[oY + 4] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oY + 5] = _cubeVertices[Index.Encode(x - 1, y, z)]; // unique
        }
        else
        {
            meshCpu.Vertices[oY + 5] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oY + 4] = _cubeVertices[Index.Encode(x - 1, y, z - 1)];
            meshCpu.Vertices[oY + 3] = _cubeVertices[Index.Encode(x, y, z - 1)]; // unique

            meshCpu.Vertices[oY + 2] = _cubeVertices[Index.Encode(x - 1, y, z - 1)];
            meshCpu.Vertices[oY + 1] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oY + 0] = _cubeVertices[Index.Encode(x - 1, y, z)]; // unique
        }
    }

    int iZ = _edgeMapZ.Index.Encode(x, y, z);
    int oZ = _edgeMapZ.IndexOffset[iZ];
    if (oZ >= 0)
    {
        if (_edgeMapZ.Direction[iZ] == 1)
        {
            meshCpu.Vertices[oZ + 0] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oZ + 1] = _cubeVertices[Index.Encode(x - 1, y - 1, z)];
            meshCpu.Vertices[oZ + 2] = _cubeVertices[Index.Encode(x - 1, y, z)]; // unique

            meshCpu.Vertices[oZ + 3] = _cubeVertices[Index.Encode(x - 1, y - 1, z)];
            meshCpu.Vertices[oZ + 4] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oZ + 5] = _cubeVertices[Index.Encode(x, y - 1, z)]; // unique
        }
        else
        {
            meshCpu.Vertices[oZ + 5] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oZ + 4] = _cubeVertices[Index.Encode(x - 1, y - 1, z)];
            meshCpu.Vertices[oZ + 3] = _cubeVertices[Index.Encode(x - 1, y, z)]; // unique

            meshCpu.Vertices[oZ + 2] = _cubeVertices[Index.Encode(x - 1, y - 1, z)];
            meshCpu.Vertices[oZ + 1] = _cubeVertices[Index.Encode(x, y, z)];
            meshCpu.Vertices[oZ + 0] = _cubeVertices[Index.Encode(x, y - 1, z)]; // unique
        }
    }
}

void MeshGenerator::CalculateEdge(int index)
{
    float d0 = _cachedSDF->CachedDistances[index];
    glm::vec3 p0 = _cachedSDF->CachedPositions[index];

    int x, y, z;
    _cachedSDF->Index.Decode(index, x, y, z);

    CalculateEdgeX(x, y, z, d0, p0);
    CalculateEdgeY(x, y, z, d0, p0);
    CalculateEdgeZ(x, y, z, d0, p0);
}

void MeshGenerator::CalculateEdgeX(const int& x, const int& y, const int& z, const float& d0, const glm::vec3& p0)
{
    if (x >= _edgeMapX.Index.SizeX)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x + 1, y, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapX.Index.Encode(x, y, z);

    _edgeMapX.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapX.Direction[index] = NonZeroSign(d1);

    if (y > 0 && z > 0 && y < Index.SizeY && z < Index.SizeZ)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapX.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(x, y, z)] = true;
        _cubeCheck[Index.Encode(x, y - 1, z)] = true;
        _cubeCheck[Index.Encode(x, y - 1, z - 1)] = true;
        _cubeCheck[Index.Encode(x, y, z - 1)] = true;
    }
    else
    {
        _edgeMapX.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeY(const int& x, const int& y, const int& z, const float& d0, const glm::vec3& p0)
{
    if (y >= _edgeMapY.Index.SizeY)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y + 1, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapY.Index.Encode(x, y, z);

    _edgeMapY.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapY.Direction[index] = NonZeroSign(d1);

    if (x > 0 && z > 0 && x < Index.SizeX && z < Index.SizeZ)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapY.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(x, y, z)] = true;
        _cubeCheck[Index.Encode(x - 1, y, z)] = true;
        _cubeCheck[Index.Encode(x - 1, y, z - 1)] = true;
        _cubeCheck[Index.Encode(x, y, z - 1)] = true;
    }
    else
    {
        _edgeMapY.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeZ(const int& x, const int& y, const int& z, const float& d0, const glm::vec3& p0)
{
    if (z >= _edgeMapZ.Index.SizeZ)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y, z + 1, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = _edgeMapZ.Index.Encode(x, y, z);

    _edgeMapZ.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapZ.Direction[index] = NonZeroSign(d1);

    if (x > 0 && y > 0 && x < Index.SizeX && y < Index.SizeY)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapZ.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Index.Encode(x, y, z)] = true;
        _cubeCheck[Index.Encode(x - 1, y, z)] = true;
        _cubeCheck[Index.Encode(x - 1, y - 1, z)] = true;
        _cubeCheck[Index.Encode(x, y - 1, z)] = true;
    }
    else
    {
        _edgeMapZ.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateVertex(int index)
{
    int x, y, z;
    Index.Decode(index, x, y, z);
    if (!_cubeCheck[index])
        return;

    int cnt = 0;
    glm::vec3 sum = glm::vec3(0.0f);

    AggregateEdge(_edgeMapX, x, y, z, sum, cnt);
    AggregateEdge(_edgeMapX, x, y + 1, z, sum, cnt);
    AggregateEdge(_edgeMapX, x, y + 1, z + 1, sum, cnt);
    AggregateEdge(_edgeMapX, x, y, z + 1, sum, cnt);

    AggregateEdge(_edgeMapY, x, y, z, sum, cnt);
    AggregateEdge(_edgeMapY, x + 1, y, z, sum, cnt);
    AggregateEdge(_edgeMapY, x + 1, y, z + 1, sum, cnt);
    AggregateEdge(_edgeMapY, x, y, z + 1, sum, cnt);

    AggregateEdge(_edgeMapZ, x, y, z, sum, cnt);
    AggregateEdge(_edgeMapZ, x + 1, y, z, sum, cnt);
    AggregateEdge(_edgeMapZ, x + 1, y + 1, z, sum, cnt);
    AggregateEdge(_edgeMapZ, x, y + 1, z, sum, cnt);

    _cubeVertices[index] = sum / (float)cnt;
}

void MeshGenerator::AggregateEdge(const EdgeMap& edgeMap, const int& x, const int& y, const int& z, glm::vec3& sum, int& cnt)
{
    int iX = edgeMap.Index.Encode(x, y, z);
    if (edgeMap.IndexOffset[iX] != EDGE_NO_SIGN_CHANGE)
    {
        sum += edgeMap.Intersection[iX];
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
